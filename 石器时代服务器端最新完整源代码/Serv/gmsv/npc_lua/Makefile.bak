INCFLAGS=-I. -I../include -I../lua
FLTFLAGS="-s 1331200"   
export FLTFLAGS

#CFLAGS=-w -O3 -g $(INCFLAGS)
#CHOST="i686-pc-linux-gnu"
#CFLAGS=-march=pentium4 -w -O3 -pipe -mmmx -msse -msse2 $(INCFLAGS)
#CXXFLAGS="-march=pentium4 -O3 -pipe -fomit-frame-pointer"
CFLAGS=-w -Wall -pipe $(INCFLAGS)

PROG=libnpc_lua.a

SRC=npc_lua.c npc_lua_ex.c npc_lua_item.c npc_lua_nlg.c npc_lua_nl.c npc_lua_obj.c npc_lua_char.c npc_lua_battle.c npc_lua_game.c npc_lua_map.c npc_lua_spell.c npc_lua_sql.c

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
	
depend:
	$(MV) Makefile Makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' Makefile.bak>Makefile
	$(CC) $(INCFLAGS) -M $(SRC) >> Makefile 

clean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~

distclean:
	$(RM) $(PROG)
	$(RM) $(OBJ)
	$(RM) *~
	$(MV) Makefile Makefile.bak
	$(SED) -ne '1,/^# DO NOT DELETE THIS LINE/p' Makefile.bak>Makefile
	$(RM) Makefile.bak

# DO NOT DELETE THIS LINE
npc_lua.o: npc_lua.c ../include/npc_lua.h ../include/npc_lua_interface.h \
  ../include/version.h ../include/correct_bug.h ../include/common.h \
  /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/char_data.h ../include/anim_tbl.h ../include/object.h \
  ../include/battle.h ../include/trade.h ../include/npcutil.h \
  ../include/item.h ../include/readmap.h
npc_lua_ex.o: npc_lua_ex.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h
npc_lua_item.o: npc_lua_item.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/item.h
npc_lua_nlg.o: npc_lua_nlg.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/configfile.h
npc_lua_nl.o: npc_lua_nl.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/autil.h \
  ../include/configfile.h ../include/enemy.h
npc_lua_obj.o: npc_lua_obj.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h
npc_lua_char.o: npc_lua_char.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/longzoro/sasql.h \
  ../include/longzoro/version.h ../include/pet_skill.h ../include/enemy.h \
  ../include/family.h
npc_lua_battle.o: npc_lua_battle.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h
npc_lua_game.o: npc_lua_game.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/char_data.h ../include/anim_tbl.h ../include/object.h \
  ../include/battle.h ../include/trade.h ../include/npcutil.h \
  ../include/item.h ../include/readmap.h ../include/saacproto_cli.h \
  ../include/saacproto_util.h /usr/include/strings.h \
  /usr/include/unistd.h /usr/include/bits/posix_opt.h \
  /usr/include/bits/confname.h /usr/include/getopt.h ../include/family.h
npc_lua_map.o: npc_lua_map.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/item.h
npc_lua_spell.o: npc_lua_spell.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/anim_tbl.h ../include/object.h ../include/net.h \
  ../include/npcutil.h ../include/npc_eventaction.h ../include/battle.h \
  ../include/trade.h ../include/readmap.h ../include/pet_skill.h \
  ../include/profession_skill.h ../include/magic_base.h
npc_lua_sql.o: npc_lua_sql.c ../include/npc_lua.h \
  ../include/npc_lua_interface.h ../include/version.h \
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
  ../include/longzoro/debug.h ../include/../lua/lua.h \
  ../include/../lua/luaconf.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/limits.h \
  /usr/lib/gcc/i386-redhat-linux/4.1.2/include/syslimits.h \
  /usr/include/limits.h /usr/include/bits/posix1_lim.h \
  /usr/include/bits/local_lim.h /usr/include/linux/limits.h \
  /usr/include/bits/posix2_lim.h ../include/../lua/lauxlib.h \
  ../include/../lua/lua.h /usr/include/string.h /usr/include/stdlib.h \
  /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h \
  /usr/include/bits/endian.h /usr/include/sys/select.h \
  /usr/include/bits/select.h /usr/include/bits/sigset.h \
  /usr/include/bits/time.h /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h /usr/include/alloca.h \
  /usr/include/assert.h ../include/util.h /usr/include/sys/time.h \
  ../include/char.h ../include/char_base.h ../include/skill.h \
  ../include/util.h ../include/title.h ../include/addressbook.h \
  ../include/net.h ../include/autil.h /usr/include/netinet/in.h \
  /usr/include/stdint.h /usr/include/sys/socket.h /usr/include/sys/uio.h \
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
  ../lua/lauxlib.h ../lua/lualib.h ../lua/lua.h ../include/char_data.h \
  ../include/item.h ../include/char.h ../include/char_base.h \
  ../include/char_data.h ../include/anim_tbl.h ../include/object.h \
  ../include/battle.h ../include/trade.h ../include/npcutil.h \
  ../include/item.h ../include/readmap.h ../include/longzoro/sasql.h \
  ../include/longzoro/version.h
