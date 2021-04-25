
# 定义CC编译器和编译开关
CCC       = /opt/SUNWspro/bin/CC
IGWB_ROOT = ../../
CCFLAGS   = -mt -I. -I$(ACE_ROOT) -I$(IGWB_ROOT) -I$(SNMP_ROOT)/agent++/include -I$(SNMP_ROOT)/snmp++/include -D_NO_SNMPv3


#定义MAKE规则
OBJ        = ../lib/libalarm_snmp_agent.so	
TMP_OBJ    = .obj/
SOURCES    = $(wildcard *.cpp *.c)
OBJS       = $(patsubst %.cpp, .obj/%.o, $(patsubst %.c, .obj/%.o, $(SOURCES))) 
SNMP_ROOT  = /export/home/snmp
IGWB_LIBS  = -L../lib -ltoolbox -lframe -lcomm
SNMP_LIBS  = -L$(SNMP_ROOT)/snmp++/lib -L$(SNMP_ROOT)/agent++/lib -lsnmp++ -lagent++
	
# 连接最终目标文件 
$(OBJ):mkdir $(OBJS) 
	$(CCC) -G -o $(OBJ) $(OBJS) $(IGWB_LIBS) $(SNMP_LIBS) 

# 编译目标文件 
.obj/%.o : %.cpp
	$(CCC) -c $(CCFLAGS) $< -o $@

# 清除生成的目标文件，在利用"make clean"命令时有效 
clean:
	rm -f $(OBJS) $(OBJ)
	rm -f core

mkdir:
	if (test -d .obj) ; then pwd ; else mkdir .obj ; fi

