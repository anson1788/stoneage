export CC=gcc
export RM=rm -f
export AR=ar cr
export MV=mv
export RANLIB=ranlib
export SED=sed


export SHELL=/bin/sh

INCFLAGS=-I. -I../include -I../lua

CFLAGS=-w -O3 -Wall -pipe $(INCFLAGS)
#Pentium 4 (Intel)
#CFLAGS=-march=pentium4 -O3 -pipe -fomit-frame-pointer $(INCFLAGS)
#Athlon XP (AMD)
#CFLAGS="-march=athlon-xp -O3 -pipe -fomit-frame-pointer" $(INCFLAGS)
#Athlon64£º
#CFLAGS="-O2 -march=athlon64 -ftracer -fprefetch-loop-arrays -pipe" $(INCFLAGS)

PROG=libmylua.a

SRC=mylua.c ablua.c init.c charbase.c npcbase.c function.c lssprotobase.c battlebase.c objectbase.c mapbase.c otherbase.c configbase.c itembase.c magicbase.c offlinebase.c netbase.c enemytempbase.c sasqlbase.c saacproto.c family.c petskill.c profession_skill.c

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
	$(MV) makefile Makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' makefile.bak>makefile
	$(RM) makefile.bak

# DO NOT DELETE THIS LINE
mylua.o: mylua.c /usr/include/signal.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/include/bits/sigset.h /usr/include/bits/types.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/typesizes.h /usr/include/bits/signum.h \
  /usr/include/time.h /usr/include/bits/siginfo.h \
  /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h \
  /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h \
  /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h /usr/include/alloca.h \
  /usr/include/string.h /usr/include/dirent.h /usr/include/bits/dirent.h \
  /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h \
  /usr/include/linux/limits.h ../include/autil.h ../include/version.h \
  ../include/correct_bug.h ../include/common.h /usr/include/errno.h \
  /usr/include/bits/errno.h /usr/include/linux/errno.h \
  /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
  ../include/buf.h ../include/util.h /usr/include/sys/time.h ../lua/lua.h \
  ../lua/luaconf.h /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix2_lim.h ../lua/lauxlib.h \
  ../lua/lua.h ../lua/lualib.h ../include/longzoro/version.h \
  ../include/mylua/mylua.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h
ablua.o: ablua.c /usr/include/pthread.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/include/endian.h /usr/include/bits/endian.h /usr/include/sched.h \
  /usr/include/bits/types.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/bits/typesizes.h /usr/include/time.h \
  /usr/include/bits/sched.h /usr/include/bits/time.h \
  /usr/include/signal.h /usr/include/bits/sigset.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h \
  /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/dirent.h /usr/include/bits/dirent.h \
  /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h \
  /usr/include/linux/limits.h /usr/include/string.h \
  /usr/include/sys/types.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/sys/sysmacros.h \
  /usr/include/sys/stat.h /usr/include/bits/stat.h ../include/autil.h \
  ../include/version.h ../include/correct_bug.h ../include/common.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/stdlib.h /usr/include/alloca.h ../include/mylua/mylua.h \
  ../lua/lua.h ../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix2_lim.h ../lua/lauxlib.h \
  ../lua/lua.h ../lua/lualib.h ../include/mylua/base.h \
  ../include/char_base.h ../include/skill.h ../include/util.h \
  ../include/title.h ../include/addressbook.h ../include/net.h \
  ../include/autil.h /usr/include/netinet/in.h /usr/include/stdint.h \
  /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/bits/uio.h /usr/include/bits/socket.h \
  /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
  /usr/include/asm/sockios.h /usr/include/bits/in.h \
  /usr/include/bits/byteswap.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  ../include/link.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/longzoro/version.h
init.o: init.c ../lua/lua.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h /usr/include/bits/stdio_lim.h \
  /usr/include/bits/sys_errlist.h ../include/mylua/base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  /usr/include/bits/sockaddr.h /usr/include/asm/socket.h \
  /usr/include/asm/sockios.h /usr/include/bits/in.h \
  /usr/include/bits/byteswap.h /usr/include/signal.h \
  /usr/include/bits/signum.h /usr/include/bits/siginfo.h \
  /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h \
  /usr/include/asm/sigcontext.h /usr/include/bits/sigstack.h \
  /usr/include/bits/sigthread.h /usr/include/pthread.h \
  /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h
charbase.o: charbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../include/char.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/battle.h ../include/trade.h ../include/object.h \
  ../include/char_base.h ../include/enemy.h ../include/mylua/base.h \
  ../include/char_base.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/npcutil.h ../include/readmap.h ../include/log.h \
  ../include/pet.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/longzoro/version.h ../include/handletime.h \
  ../include/item_event.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/shop.h ../include/mylua/mylua.h
npcbase.o: npcbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/battle.h ../include/net.h ../include/common.h \
  /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/autil.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/sys/time.h /usr/include/time.h \
  /usr/include/bits/time.h /usr/include/sys/select.h \
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
  /usr/include/bits/setjmp.h ../include/link.h ../include/trade.h \
  ../include/util.h /usr/include/stdlib.h /usr/include/alloca.h \
  ../lua/lua.h ../lua/luaconf.h ../lua/lauxlib.h ../lua/lua.h \
  ../lua/lualib.h ../include/char_base.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../include/npccreate.h \
  ../include/object.h ../include/char.h ../include/char_base.h \
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/enemy.h ../include/npc_eventaction.h ../lua/lua.h \
  ../include/longzoro/version.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/npcutil.h ../include/log.h
function.o: function.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
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
  ../include/item.h ../include/configfile.h ../include/battle.h \
  ../include/trade.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/longzoro/version.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/mylua/mylua.h
lssprotobase.o: lssprotobase.c /usr/include/string.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/math.h /usr/include/bits/huge_val.h \
  /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h \
  ../include/char.h ../include/common.h /usr/include/errno.h \
  /usr/include/bits/errno.h /usr/include/linux/errno.h \
  /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h ../include/char_base.h \
  ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
battlebase.o: battlebase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/debug.h \
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
  ../include/enemy.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/battle.h ../include/trade.h \
  ../include/battle_command.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/mylua/mylua.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/mylua/base.h ../include/char_base.h
objectbase.o: objectbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/object.h ../include/longzoro/version.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/mylua/base.h \
  ../include/char_base.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h
mapbase.o: mapbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  ../include/char.h ../include/common.h /usr/include/errno.h \
  /usr/include/bits/errno.h /usr/include/linux/errno.h \
  /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h ../include/char_base.h \
  ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/object.h ../include/readmap.h ../include/map_deal.h \
  ../include/char_base.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
otherbase.o: otherbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/sys/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h ../include/char.h ../include/common.h \
  /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h \
  /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../include/util.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/npc_scheduleman.h \
  ../include/mylua/mylua.h
configbase.o: configbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../include/char.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/configfile.h
itembase.o: itembase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../include/longzoro/debug.h \
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
  ../include/mylua/base.h ../include/char_base.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/item.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/mylua/mylua.h
magicbase.o: magicbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_talk.h ../include/common.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h ../include/net.h \
  ../include/autil.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h /usr/include/netinet/in.h \
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
  ../include/mylua/base.h ../include/char_base.h ../include/skill.h \
  ../include/title.h ../include/addressbook.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/mylua/ablua.h ../include/magic_base.h \
  ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/mylua/mylua.h
offlinebase.o: offlinebase.c /usr/include/string.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/longzoro/version.h ../include/longzoro/debug.h \
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
  ../include/enemy.h ../include/object.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/battle.h ../include/trade.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/mylua/mylua.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h
netbase.o: netbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../lua/lua.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h ../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../lua/lauxlib.h /usr/include/stdio.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  ../lua/lua.h ../lua/lualib.h ../include/net.h ../include/common.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/autil.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/debug.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h \
  /usr/include/bits/socket.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/log.h
enemytempbase.o: enemytempbase.c /usr/include/string.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  ../include/char.h ../include/common.h /usr/include/errno.h \
  /usr/include/bits/errno.h /usr/include/linux/errno.h \
  /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
  /usr/include/asm-generic/errno-base.h ../include/char_base.h \
  ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/readmap.h ../include/map_deal.h ../include/char_base.h \
  ../include/enemy.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/lssproto_serv.h \
  ../include/lssproto_util.h /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
sasqlbase.o: sasqlbase.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../lua/lua.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h ../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/longzoro/version.h \
  ../include/longzoro/longzoro.h ../lua/lauxlib.h /usr/include/stdio.h \
  /usr/include/bits/types.h /usr/include/bits/typesizes.h \
  /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
  /usr/include/bits/wchar.h /usr/include/gconv.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  ../lua/lua.h ../lua/lualib.h ../include/net.h ../include/common.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/autil.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/debug.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h \
  /usr/include/bits/socket.h /usr/include/bits/sockaddr.h \
  /usr/include/asm/socket.h /usr/include/asm/sockios.h \
  /usr/include/bits/in.h /usr/include/bits/byteswap.h \
  /usr/include/signal.h /usr/include/bits/signum.h \
  /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
  /usr/include/bits/sigcontext.h /usr/include/asm/sigcontext.h \
  /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h \
  /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h \
  /usr/include/bits/setjmp.h ../include/link.h ../include/log.h \
  ../include/longzoro/sasql.h ../include/longzoro/version.h \
  /usr/include/mysql/mysql.h /usr/include/mysql/mysql_version.h \
  /usr/include/mysql/mysql_com.h /usr/include/mysql/mysql_time.h \
  /usr/include/mysql/typelib.h /usr/include/mysql/my_alloc.h \
  /usr/include/mysql/my_list.h
saacproto.o: saacproto.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../include/char.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/mylua/base.h ../include/char_base.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h \
  ../lua/lauxlib.h ../lua/lualib.h ../include/saacproto_cli.h \
  ../include/saacproto_util.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h
family.o: family.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h ../include/char.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/char_base.h ../include/version.h ../include/correct_bug.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/skill.h ../include/util.h \
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
  ../include/char_data.h ../include/item.h ../include/char.h \
  ../include/char_base.h ../include/family.h ../include/mylua/base.h \
  ../include/char_base.h ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h \
  ../include/saacproto_cli.h ../include/saacproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h
petskill.o: petskill.c /usr/include/string.h /usr/include/features.h \
  /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
  /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/version.h ../include/correct_bug.h ../include/common.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/pet_skill.h ../include/util.h \
  /usr/include/sys/time.h /usr/include/time.h /usr/include/bits/time.h \
  /usr/include/sys/select.h /usr/include/bits/select.h \
  /usr/include/bits/sigset.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/mylua/base.h ../include/char_base.h ../include/version.h \
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
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/item.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/mylua/mylua.h
profession_skill.o: profession_skill.c /usr/include/string.h \
  /usr/include/features.h /usr/include/sys/cdefs.h \
  /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
  /usr/include/gnu/stubs-32.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stddef.h \
  ../include/common.h /usr/include/stdio.h /usr/include/bits/types.h \
  /usr/include/bits/typesizes.h /usr/include/libio.h \
  /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/stdarg.h \
  /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
  /usr/include/errno.h /usr/include/bits/errno.h \
  /usr/include/linux/errno.h /usr/include/asm/errno.h \
  /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h \
  ../include/version.h ../include/correct_bug.h ../include/common.h \
  ../include/longzoro/version.h ../include/longzoro/longzoro.h \
  ../include/longzoro/debug.h ../include/profession_skill.h \
  ../include/version.h ../include/util.h /usr/include/sys/time.h \
  /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  ../include/skill.h ../include/mylua/base.h ../include/char_base.h \
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
  ../lua/lua.h ../lua/lauxlib.h ../lua/lualib.h ../include/char.h \
  ../include/char_base.h ../include/char_data.h ../include/item.h \
  ../include/char.h ../include/item.h ../lua/lua.h ../lua/lauxlib.h \
  ../lua/lualib.h ../include/lssproto_serv.h ../include/lssproto_util.h \
  /usr/include/strings.h /usr/include/unistd.h \
  /usr/include/bits/posix_opt.h /usr/include/bits/confname.h \
  /usr/include/getopt.h ../include/mylua/mylua.h
