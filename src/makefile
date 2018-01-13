PROGRAM=light_ctr_svr

flag = debug

COMMON_INC_DIR = ../include
LIBEV_INC = $(COMMON_INC_DIR)/libev

OBJ_DIR = ../objs
DEV_LIB = ../libs
BIN_DIR = ../bin

MODULES = Log BaseLayer DomainLayer 

MODULES_PATH =  $(addprefix $(OBJ_DIR)/,$(MODULES))

SVR_SRC = $(foreach MOD_DIR, $(MODULES), $(wildcard $(MOD_DIR)/*.cpp))
SVR_OBJ = $(addprefix $(OBJ_DIR)/, $(subst .cpp,.o, $(SVR_SRC)))
SVR_DEP = $(subst .o,.d, $(SVR_OBJ))
SVR = $(BIN_DIR)/$(PROGRAM)

MAKE_OBJ_PATH := $(shell mkdir -p $(OBJ_DIR) $(MODULES_PATH))

INC_FLAGS = -I$(COMMON_INC_DIR) -I$(LIBEV_INC) 
INC_FLAGS += $(addprefix -I./, $(MODULES))
LD_FLAGS = -L$(DEV_LIB) -lev -Wl,-rpath=$(DEV_LIB) 

ifeq ($(flag), debug)
CC_FLAGS = -g $(INC_FLAGS) -DDEBUG
else
CC_FLAGS = -g $(INC_FLAGS)
endif


CC = g++ -std=c++11 

NOT_DEPS_GOALS = clean rebuild

.PHONY: all clean build rebuild install

all: build

build: $(SVR) 

rebuild: clean build

$(warning $(SVR_DEP))
#ifeq ($(findstring $(MAKECMDGOALS), $(NOT_DEPS_GOALS)), )
#    -include $(SVR_DEP)
#endif

$(OBJ_DIR)/%.d: %.cpp
	$(CC) -MM $(CC_FLAGS) $< -MT"$(@:.d=.o)" -MT"$@" -o $@

$(SVR): $(SVR_OBJ)
	@echo $(SVR_OBJ)
	@echo $(SVR)
	$(CC) -o $@ $(SVR_OBJ) $(LD_FLAGS)
    
$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CC_FLAGS) 

clean:
	rm -f $(SVR_DEP) $(SVR_OBJ) $(SVR)

