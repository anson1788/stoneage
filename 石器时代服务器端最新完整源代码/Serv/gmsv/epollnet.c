/*
 * epollnet.c
 *
 *  Created on: 2015年10月22日
 *      Author: hult
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include "net.h"
#include "epollnet.h"
#include "configfile.h"
#include "version.h"

#ifdef _EPOLL_ET_MODE
struct epoll_event *events = NULL;
int epollFd = -1;

int epoll_init();
int epoll_socket(int domain, int type, int protocol);
int epoll_cleanup();

//绑定监听端口
int epoll_bind(int port) {

	struct sockaddr_in listenAddr;
	int sfd = -1;

	if (-1 == epoll_init()) {
		printf("epoll_init err\n");
		return -1;
	}

	if ((sfd = epoll_socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("epoll_socket err\n");
		epoll_cleanup();
		return -1;
	}

	listenAddr.sin_family = AF_INET;
	listenAddr.sin_port = htons(port);
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(sfd, (struct sockaddr*) &listenAddr, sizeof(listenAddr))) {
		printf("bind err %d\n", errno);
		epoll_cleanup();
		return -1;
	}

	if (-1 == listen(sfd, 1024)) {
		printf("listen err\n");
		epoll_cleanup();
		return -1;
	}

	//Add bindedfd into epoll
	if (-1 == epoll_new_conn(sfd)) {
		printf("eph_new_conn err\n");
		close(sfd);
		epoll_cleanup();
		return -1;
	}

	return sfd;
}

int epoll_init() {
	if (!(events = (struct epoll_event*) malloc(
			ConnectLen * sizeof(struct epoll_event)))) {
		return -1;
	}

	if ((epollFd = epoll_create(ConnectLen)) < 0) {
		return -1;
	}

	return 0;
}

int epoll_socket(int domain, int type, int protocol) {
	int sockFd = -1;

	if ((sockFd = socket(domain, type, protocol)) < 0) {
		return -1;
	}
	//端口复用
	if (getReuseaddr()) {
		int sendbuff;
		setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, (char *) &sendbuff,
				sizeof(sendbuff));
	}
	//非阻塞
	if (epoll_set_nonblock(sockFd) < 0) {
		return -1;
	}

	return sockFd;
}

int epoll_set_nonblock(int fd) {
	int flag = -1;
	//Set Socket to non-block
	if ((flag = fcntl(fd, F_GETFL, 0)) < 0
			|| fcntl(fd, F_SETFL, flag | O_NONBLOCK) < 0) {
		close(fd);
		return -1;
	}
	return 0;
}

int epoll_new_conn(int sfd) {
	struct epoll_event epollEvent;
	memset(&epollEvent, 0, sizeof(struct epoll_event));
	epollEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
	epollEvent.data.fd = sfd;

	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, sfd, &epollEvent) < 0) {
		return -1;
	}
	return 0;
}

int epoll_close_conn(int sfd) {
	struct epoll_event epollEvent;
	memset(&epollEvent, 0, sizeof(struct epoll_event));
	epollEvent.events = NULL;
	epollEvent.data.fd = sfd;

	if (epoll_ctl(epollFd, EPOLL_CTL_DEL, sfd, &epollEvent) < 0) {
		return -1;
	}
	return 0;
}

int epoll_mod_read(int sfd) {
	struct epoll_event epollEvent;
	memset(&epollEvent, 0, sizeof(struct epoll_event));
	epollEvent.events = EPOLLIN | EPOLLHUP | EPOLLERR;
	epollEvent.data.fd = sfd;

	if (epoll_ctl(epollFd, EPOLL_CTL_MOD, sfd, &epollEvent) < 0) {
		return -1;
	}
	return 0;
}

int epoll_mod_write(int sfd) {
	struct epoll_event epollEvent;
	memset(&epollEvent, 0, sizeof(struct epoll_event));
	epollEvent.events = EPOLLOUT | EPOLLHUP | EPOLLERR;
	epollEvent.data.fd = sfd;

	if (epoll_ctl(epollFd, EPOLL_CTL_MOD, sfd, &epollEvent) < 0) {
		return -1;
	}
	return 0;
}

int epoll_cleanup() {
	free(events);
	close(epollFd);
	return 0;
}

//将acfd添加到epoll中
int epoll_add_acfd(int acfd) {
	if (epoll_set_nonblock(acfd) == -1) {
		return -1;
	}

	if (epoll_new_conn(acfd) == -1) {
		return -1;
	}
	return 0;
}

//读封包处理
void PacketWrap_Thread(int state) {
	pthread_detach(pthread_self());
	printf("Thread %d Start\n", state);
	while (1) {
		int i;
		for (i = state; i < ConnectLen; i += _EPOLL_POOL_COUNT) {
			Dispatch_read_buffer(i, state);
			usleep(1);
		}
	}
}
//启动封包处理线程
int Start_PacketWrapper() {
	int i, ret;
	pthread_t thread;
	for (i = 0; i < _EPOLL_POOL_COUNT; i++) {
		ret = pthread_create(&thread, NULL, PacketWrap_Thread, (void *) i);
		if (ret != 0) {
			return ret;
		}
	}
	return 0;
}
//主循环
void EpollLoop_Thread() {
	pthread_detach(pthread_self());
	//LOOP
	while (1) {
		int n, i;
		n = epoll_wait(epollFd, events, ConnectLen, -1);
		for (i = 0; i < n; i++) {
			if ((events[i].events & EPOLLERR)
					|| (events[i].events & EPOLLHUP)) {
				CONNECT_endOne_debug(events[i].data.fd);
				continue;
			} else if (events[i].data.fd == bindedfd) {
				while (1) {
					if (doSocketAccept() == -1) {
						break;
					}
				}
			} else if (events[i].events & EPOLLIN) {
				//接收
				doSocketRecv(events[i]);
			} else if (events[i].events & EPOLLOUT) {
				//发送
				doSocketSend(events[i]);
			}
		}
		usleep(1);
	}
	epoll_cleanup();
}

//启动epoll主循环线程
int Start_Epoll_Loop() {
	pthread_t thread;
	int ret = pthread_create(&thread, NULL, EpollLoop_Thread, NULL);
	return ret;
}

//void EpollSendLoop_Thread() {
//	pthread_detach(pthread_self());
//	while (1) {
//		doSocketSend();
//	}
//}
//
////启动epoll发送线程
//int Start_Epoll_SendLoop() {
//	pthread_t thread;
//	int ret = pthread_create(&thread, NULL, EpollSendLoop_Thread, NULL);
//	return ret;
//}

#endif
