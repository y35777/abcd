# iGateway Bill V200R002B02 om_proc Makefile
# 定义编译器和编译开关
include ../../platform.in
include ../../debug.in
include ../../product.in

# 定义输出规则
OUTPUT_OBJ = om_proc
LINK_LIB = -lalarm -lalarm_util -lmml -lbackup -lperf -lquerybrowse -llog -lcdrstat -lframe -lcomm -ltoolbox -lformat -ltoolbase
ifeq ($(PRODUCT_TYPE),CCS)
LINK_LIB := -lcoagent $(LINK_LIB)
else
ifeq ($(PRODUCT_TYPE),NGN)
LINK_LIB := -lbilltask $(LINK_LIB)
endif
endif
include ../../app.in
include ../../rule.in
