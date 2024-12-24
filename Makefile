# 定义变量
CC := g++
CFLAGS := -Wall -Werror -std=c++11
LDFLAGS := 

# 定义源文件和目标文件路径
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin

# 定义源文件和目标文件
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TARGET := $(BINDIR)/http_server

# 创建目标文件目录
$(OBJDIR) $(BINDIR):
    mkdir -p $@

# 编译规则
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
    $(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# 链接规则
$(TARGET): $(OBJS) | $(BINDIR)
    $(CC) $(OBJS) $(LDFLAGS) -o $@

# 定义默认目标
default: $(TARGET)

# 清理规则
clean:
    rm -rf $(OBJDIR) $(BINDIR)