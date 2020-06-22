/*
 * epollnet.h
 *
 *  Created on: 2015年10月23日
 *      Author: hult
 */

#ifndef EPOLLNET_H_
#define EPOLLNET_H_
#ifdef _EPOLL_ET_MODE
//绑定epoll监听端口
int epoll_bind(int port);
//将acfd添加到epoll中
int epoll_add_acfd(int acfd);
//非阻塞
int epoll_set_nonblock(int fd) ;
//注册到epoll
int epoll_new_conn(int sfd) ;
//从epoll中移除
int epoll_close_conn(int sfd);
//读epoll
int epoll_mod_read(int sfd) ;
//写epoll
int epoll_mod_write(int sfd) ;

//启动封包处理线程
int Start_PacketWrapper();
//启动epoll主循环线程
int Start_Epoll_Loop();
//int Start_Epoll_SendLoop();
#endif
#endif /* EPOLLNET_H_ */
