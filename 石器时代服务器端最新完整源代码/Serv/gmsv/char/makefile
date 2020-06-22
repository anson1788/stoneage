export CC=gcc
export RM=rm -f
export AR=ar cr
export MV=mv
export RANLIB=ranlib
export SED=sed


export SHELL=/bin/sh

INCFLAGS=-I.. -I../include -I../lua

CFLAGS=-w -O3 -Wall -pipe $(INCFLAGS)
#Pentium 4 (Intel)
#CFLAGS=-march=pentium4 -O3 -pipe -fomit-frame-pointer $(INCFLAGS)
#Athlon XP (AMD)
#CFLAGS="-march=athlon-xp -O3 -pipe -fomit-frame-pointer" $(INCFLAGS)
#Athlon64£º
#CFLAGS="-O2 -march=athlon64 -ftracer -fprefetch-loop-arrays -pipe" $(INCFLAGS)

PROG=libchar.a

SRC=char_base.c char.c char_event.c char_data.c skill.c title.c\
addressbook.c chatmagic.c event.c char_walk.c encount.c pet.c \
enemy.c pet_event.c char_talk.c char_party.c char_item.c \
chatroom.c petmail.c trade.c family.c char_angel.c

OBJ=$(SRC:.c=.o)

ifeq (0,$(MAKELEVEL))
CC=gcc
RM=rm -f
AR=ar cr
MV=mv
RANLIB=ranlib
SED=sed
SHELL=/bin/sh
endif

all: $(PROG)

$(PROG): $(OBJ)
	$(RM) $(PROG)
	$(AR) $(PROG) $(OBJ)
	$(RANLIB) $(PROG)

dos2unix:
	dos2unix $(SRC) makefile
	
chmod:
	chmod 777 $(SRC) makefile
	
depend:
	$(MV) makefile makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak>makefile
	$(CC) $(INCFLAGS) -M $(SRC) >> makefile 
	chmod 777 makefile

clean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~

distclean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~
	$(MV) makefile makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak>makefile
	$(RM) makefile.bak

# DO NOT DELETE THIS LINE
char_base.o: char_base.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h /usr/include/time.h \
  /usr/include/bits/time.h ../include/net.h ../include/autil.h \
  ../include/version.h /usr/include/sys/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h \
  /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/autil.h \
  ../include/char_base.h ../include/skill.h ../include/util.h \
  /usr/include/stdlib.h /usr/include/alloca.h ../include/title.h \
  ../include/addressbook.h ../include/net.h ../lua/lua.h ../lua/luaconf.h \
  ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/char_data.h ../include/item.h ../include/buf.h \
  ../include/magic.h ../include/function.h ../include/npccreate.h \
  ../include/configfile.h ../include/title.h ../include/pet.h \
  ../include/pet_skill.h ../include/anim_tbl.h ../include/enemy.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/family.h ../include/log.h
char.o: char.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/sys/stat.h /usr/include/bits/stat.h \
  /usr/include/ctype.h /usr/include/sys/time.h ../include/net.h \
  ../include/autil.h ../include/version.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/common.h \
  ../include/char_base.h ../include/skill.h ../include/util.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../lua/lua.h ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h \
  ../lua/lualib.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/char.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/item.h ../include/item_event.h \
  ../include/buf.h ../include/object.h ../include/map_deal.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  ../include/readmap.h ../include/handletime.h ../include/char_event.h \
  ../include/npccreate.h ../include/addressbook.h ../include/magic_base.h \
  ../include/magic.h ../include/chatmagic.h ../include/configfile.h \
  ../include/log.h ../include/anim_tbl.h ../include/encount.h \
  ../include/battle.h ../include/trade.h ../include/pet_skill.h \
  ../include/util.h ../include/enemy.h ../include/npcutil.h \
  ../include/pet.h ../include/family.h ../include/correct_bug.h \
  ../include/npc_checkman.h ../include/autil.h \
  ../include/profession_skill.h ../include/npc_autopk.h \
  ../include/longzoro/sasql.h ../include/longzoro/version.h \
  ../include/chatroom.h ../include/mylua/function.h
char_event.o: char_event.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/ctype.h /usr/include/endian.h \
  /usr/include/bits/endian.h ../include/common.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/util.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/item_event.h ../include/net.h ../include/item.h \
  ../include/object.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/string.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h ../include/util.h \
  ../include/char_data.h ../include/readmap.h ../include/map_deal.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  ../include/npccreate.h ../include/handletime.h ../include/anim_tbl.h \
  ../include/family.h
char_data.o: char_data.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h /usr/include/math.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h ../include/autil.h ../include/version.h \
  ../include/readmap.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/common.h ../include/util.h ../include/anim_tbl.h \
  ../include/battle.h ../include/net.h ../include/autil.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/trade.h \
  ../lua/lua.h ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h \
  ../lua/lualib.h ../include/char.h ../include/char_base.h \
  ../include/skill.h ../include/title.h ../include/addressbook.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_data.h ../include/buf.h ../include/item.h \
  ../include/log.h ../include/pet.h ../include/enemy.h \
  ../include/char_base.h ../include/configfile.h \
  ../include/defaultPlayer.h ls2data.dat ../include/family.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
skill.o: skill.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h ../include/util.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/battle.h ../include/trade.h ../include/profession_skill.h
title.o: title.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/ctype.h ../include/autil.h \
  ../include/version.h ../include/title.h ../include/util.h \
  /usr/include/sys/time.h ../include/char.h ../include/char_base.h \
  ../include/skill.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/item.h ../include/skill.h \
  ../include/buf.h ../include/util.h ../include/configfile.h
addressbook.o: addressbook.c ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/stdio.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/strings.h ../include/addressbook.h \
  ../include/util.h /usr/include/sys/time.h ../include/net.h \
  ../include/autil.h ../include/version.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/char.h \
  ../include/char_base.h ../include/skill.h ../include/title.h \
  ../include/addressbook.h ../lua/lua.h ../lua/luaconf.h ../lua/lauxlib.h \
  ../lua/lua.h ../lua/lualib.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/handletime.h ../include/buf.h \
  ../include/net.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  ../include/object.h ../include/battle.h ../include/trade.h \
  ../include/configfile.h ../include/npcutil.h ../include/pet.h \
  ../include/petmail.h ../include/log.h
chatmagic.o: chatmagic.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h /usr/include/ctype.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/math.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h /usr/include/malloc.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/common.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/util.h ../include/buf.h ../include/npcutil.h \
  ../include/char_base.h ../include/object.h ../include/char_data.h \
  ../include/handletime.h ../include/chatmagic.h ../include/configfile.h \
  ../include/readmap.h ../include/map_deal.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/log.h ../include/battle.h \
  ../include/trade.h ../include/pet.h ../include/enemy.h \
  ../include/encount.h ../include/magic_base.h ../include/magic.h \
  ../include/pet_skill.h ../include/pet_event.h ../include/item_gen.h \
  ../include/mclient.h ../include/npc_eventaction.h \
  ../include/map_warppoint.h ../include/npc_manorsman.h ../include/net.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  ../include/npc_scheduleman.h ../include/trade.h ../include/npccreate.h \
  ../include/npc_transmigration.h ../include/profession_skill.h \
  ../include/char_talk.h ../include/longzoro/playerquestion.h \
  ../include/npc_magiccard.h ../include/family.h ../include/petmail.h \
  ../include/npc_autopk.h ../include/mylua/ablua.h ../include/item.h \
  ../include/readnpc.h ../include/longzoro/sasql.h \
  ../include/longzoro/version.h
event.o: event.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/object.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/readmap.h \
  ../include/map_warppoint.h ../include/event.h ../include/npc_warp.h \
  ../include/npc_npcenemy.h
char_walk.o: char_walk.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/ctype.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/time.h /usr/include/bits/time.h \
  ../include/common.h ../include/handletime.h /usr/include/sys/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h ../include/object.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/alloca.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_event.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/string.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/readmap.h ../include/map_deal.h \
  ../include/npccreate.h ../include/encount.h ../include/npcutil.h \
  ../include/battle.h ../include/trade.h ../include/net.h \
  ../include/configfile.h ../include/npc_npcenemy.h \
  ../include/mylua/function.h
encount.o: encount.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h /usr/include/math.h \
  /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h \
  /usr/include/bits/mathcalls.h ../include/autil.h ../include/version.h \
  ../include/common.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h ../include/buf.h \
  ../include/char_base.h ../include/skill.h ../include/util.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/configfile.h \
  ../include/encount.h ../include/enemy.h
pet.o: pet.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/char_base.h ../include/version.h \
  ../include/skill.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/object.h \
  ../include/readmap.h ../include/map_deal.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/string.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  ../include/handletime.h ../include/pet.h ../include/battle.h \
  ../include/trade.h ../include/petmail.h ../include/log.h \
  ../include/function.h
enemy.o: enemy.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h ../include/common.h \
  ../include/autil.h ../include/version.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h ../include/buf.h \
  ../include/char_base.h ../include/skill.h ../include/util.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/configfile.h \
  ../include/encount.h ../include/enemy.h ../include/pet.h \
  ../include/enemyexptbl.h ../include/petmail.h ../include/battle.h \
  ../include/trade.h ../include/pet_skillinfo.h ../include/anim_tbl.h \
  ../include/log.h ../include/npcutil.h
pet_event.o: pet_event.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/common.h ../include/object.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/util.h \
  /usr/include/sys/time.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/configfile.h \
  ../include/handletime.h ../include/pet_event.h ../include/npcutil.h \
  ../include/log.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/string.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/npc_exchangeman.h \
  ../include/npc_eventaction.h ../include/mylua/function.h
char_talk.o: char_talk.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/ctype.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/string.h \
  ../include/handletime.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  ../include/autil.h ../include/version.h ../include/readmap.h \
  ../include/util.h /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/alloca.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/skill.h ../include/title.h \
  ../include/addressbook.h ../include/net.h ../include/autil.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/item_event.h ../include/chatmagic.h \
  ../include/battle.h ../include/trade.h ../include/log.h \
  ../include/configfile.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/saacproto_cli.h \
  ../include/saacproto_util.h ../include/family.h ../include/buf.h \
  ../include/profession_skill.h ../include/net.h ../include/char_talk.h \
  ../include/longzoro/move.h ../include/longzoro/playerquestion.h \
  ../include/mylua/function.h
char_party.o: char_party.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/readmap.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/object.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/title.h \
  ../include/addressbook.h ../include/net.h ../include/autil.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/battle.h ../include/trade.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/string.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/npcutil.h ../include/npc_bus.h \
  ../include/npc_airplane.h ../include/family.h ../include/configfile.h \
  ../include/init.h ../include/mylua/function.h
char_item.o: char_item.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/configfile.h \
  ../include/readmap.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/map_deal.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/item.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/string.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h \
  ../include/handletime.h ../include/log.h ../include/item_event.h \
  ../include/battle.h ../include/trade.h ../include/petmail.h \
  ../include/mylua/function.h
chatroom.o: chatroom.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/ctype.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/string.h ../include/readmap.h \
  ../include/util.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/alloca.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/chatmagic.h ../include/battle.h \
  ../include/trade.h ../include/log.h ../include/configfile.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/saacproto_cli.h \
  ../include/saacproto_util.h ../include/family.h ../include/chatroom.h \
  ../include/net.h ../include/util.h
petmail.o: petmail.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/configfile.h ../include/buf.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/util.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char.h ../include/char_base.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/battle.h \
  ../include/trade.h ../include/handletime.h ../include/map_deal.h \
  ../include/addressbook.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/string.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h ../include/pet.h \
  ../include/petmail.h ../include/npcutil.h ../include/log.h
trade.o: trade.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/correct_bug.h \
  /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h \
  /usr/include/bits/endian.h ../include/readmap.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/object.h ../include/char.h ../include/char_base.h \
  ../include/version.h ../include/skill.h ../include/title.h \
  ../include/addressbook.h ../include/net.h ../include/autil.h \
  /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/npcutil.h ../include/trade.h \
  ../include/log.h ../include/handletime.h ../include/buf.h \
  ../include/net.h ../include/battle.h ../include/trade.h \
  ../include/npc_bus.h ../include/char_talk.h ../include/pet_skill.h
family.o: family.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/string.h /usr/include/ctype.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/time.h \
  /usr/include/bits/time.h ../include/configfile.h ../include/readmap.h \
  ../include/util.h /usr/include/sys/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h \
  /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h \
  /usr/include/alloca.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/saacproto_cli.h \
  ../include/saacproto_util.h ../include/npcutil.h ../include/family.h \
  ../include/log.h ../include/handletime.h ../include/buf.h \
  ../include/net.h ../include/battle.h ../include/trade.h \
  ../include/npc_bus.h ../include/char_talk.h \
  ../include/npc_scheduleman.h ../include/npc_fmdengon.h \
  ../include/mylua/function.h ../include/longzoro/sasql.h \
  ../include/longzoro/version.h
char_angel.o: char_angel.c ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/sys/stat.h /usr/include/bits/stat.h \
  /usr/include/ctype.h /usr/include/sys/time.h ../include/common.h \
  ../include/char_base.h ../include/version.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h ../lua/lualib.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/char.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/item.h ../include/item_event.h \
  ../include/buf.h ../include/object.h ../include/map_deal.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  ../include/readmap.h ../include/handletime.h ../include/char_event.h \
  ../include/npccreate.h ../include/addressbook.h ../include/magic_base.h \
  ../include/magic.h ../include/chatmagic.h ../include/configfile.h \
  ../include/log.h ../include/anim_tbl.h ../include/encount.h \
  ../include/battle.h ../include/trade.h ../include/pet_skill.h \
  ../include/util.h ../include/enemy.h ../include/npcutil.h \
  ../include/pet.h ../include/family.h
