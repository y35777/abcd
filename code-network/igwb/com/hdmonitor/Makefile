
# 定义CC编译器和编译开关
SNMP_ROOT = /export/home/snmp
CCC       = /opt/SUNWspro/bin/CC
CCFLAGS   = -mt -misalign -compat=5 -I. -I$(ACE_ROOT) -I$(SNMP_ROOT)/snmp++/include -KPIC -instances=static
SYSLIBS   = -L$(ACE_ROOT)/ace  -L$(SNMP_ROOT)/snmp++/lib -L. -lCstd -lACE -lc -lCrun -lsocket -ldl -lnsl -lgen -lposix4 
INPUTS    = -I. -I$(ACE_ROOT) -DACE_NO_INLINE
LINKFLAG  = -mt -KPIC -D_POSIX_PTHREAD_SEMANTICS -R$(ACE_ROOT)/ace -R./ -xildoff
USRLIBS   = -lsnmp++


#定义MAKE规则
OBJ       = ../../depend/sun/release/libHDmonitor.so	
TMP_OBJ   = .obj/
LIB_DIR   = ./
SOURCES   = $(wildcard *.cpp *.c)
OBJS      = $(patsubst %.cpp, .obj/%.o, $(patsubst %.c, .obj/%.o, $(SOURCES))) 
	
# 连接最终目标文件 
$(OBJ):mkdir $(OBJS) 
	$(CCC) -G $(LINKFLAG) -o $(OBJ) $(OBJS) $(SYSLIBS) $(USRLIBS) 

# 编译目标文件 
.obj/%.o : %.cpp
	$(CCC) -c $(CCFLAGS) $< -o $@

# 清除生成的目标文件，在利用"make clean"命令时有效 
clean:
	rm -f $(OBJS) $(OBJ)
	rm -f core

mkdir:
	if (test -d .obj) ; then pwd ; else mkdir .obj ; fi
	if (test -d .output) ; then pwd ; else mkdir .output ; fi

