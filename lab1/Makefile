# Компилятор
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Путь к Boost (заголовочные файлы)
BOOST_ROOT = C:/libs/boost_1_66_0
INCLUDES = -I$(BOOST_ROOT)

# Путь к библиотекам - исправлено на правильный!
# Библиотеки разбросаны по разным папкам, поэтому нам нужно указать корневую папку сборки
BOOST_BUILD_ROOT = C:/libs/boost_1_66_0/build/boost/bin.v2/libs

# Мы не можем указать один путь -L, так как библиотеки в разных папках
# Вместо этого будем указывать полный путь к каждой библиотеке

# Функция для поиска .a файлов
find_lib = $(wildcard $(BOOST_BUILD_ROOT)/$(1)/build/*/*/*/*/*/libboost_$(1)-*.a)

# Проверяем наличие конкретных библиотек
BOOST_ATOMIC := $(call find_lib,atomic)
BOOST_CHRONO := $(call find_lib,chrono)
BOOST_CONTAINER := $(call find_lib,container)
BOOST_CONTEXT := $(call find_lib,context)
BOOST_COROUTINE := $(call find_lib,coroutine)
BOOST_EXCEPTION := $(call find_lib,exception)
BOOST_FILESYSTEM := $(call find_lib,filesystem)
BOOST_GRAPH := $(call find_lib,graph)
BOOST_IOSTREAMS := $(call find_lib,iostreams)
BOOST_MATH_C99 := $(call find_lib,math_c99)
BOOST_MATH_TR1 := $(call find_lib,math_tr1)
BOOST_PROGRAM_OPTIONS := $(call find_lib,program_options)
BOOST_RANDOM := $(call find_lib,random)
BOOST_SERIALIZATION := $(call find_lib,serialization)
BOOST_SIGNALS := $(call find_lib,signals)
BOOST_SYSTEM := $(call find_lib,system)
BOOST_TEST := $(call find_lib,test_exec_monitor)
BOOST_THREAD := $(call find_lib,thread)
BOOST_TIMER := $(call find_lib,timer)
BOOST_TYPE_ERASURE := $(call find_lib,type_erasure)
BOOST_WAVE := $(call find_lib,wave)
BOOST_WSERIALIZATION := $(call find_lib,wserialization)

# Формируем список объектных файлов для линковки
BOOST_LIB_OBJS = \
    $(BOOST_ATOMIC) \
    $(BOOST_CHRONO) \
    $(BOOST_CONTAINER) \
    $(BOOST_CONTEXT) \
    $(BOOST_COROUTINE) \
    $(BOOST_EXCEPTION) \
    $(BOOST_FILESYSTEM) \
    $(BOOST_GRAPH) \
    $(BOOST_IOSTREAMS) \
    $(BOOST_MATH_C99) \
    $(BOOST_MATH_TR1) \
    $(BOOST_PROGRAM_OPTIONS) \
    $(BOOST_RANDOM) \
    $(BOOST_SERIALIZATION) \
    $(BOOST_SIGNALS) \
    $(BOOST_SYSTEM) \
    $(BOOST_TEST) \
    $(BOOST_THREAD) \
    $(BOOST_TIMER) \
    $(BOOST_TYPE_ERASURE) \
    $(BOOST_WAVE) \
    $(BOOST_WSERIALIZATION)

# Системные библиотеки
SYS_LIBS = -lpthread -lws2_32

# Цель
TARGET = lab1_2.exe
SRCS = main12.cpp
OBJS = $(SRCS:.cpp=.o)

# Вывод информации о найденных библиотеках
$(info Найденные библиотеки Boost:)
$(foreach lib,$(BOOST_LIB_OBJS),$(info   $(notdir $(lib))))

all: $(TARGET)

# Линковка - подключаем найденные .a файлы напрямую
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(BOOST_LIB_OBJS) $(SYS_LIBS)
	@echo "Программа успешно скомпилирована: $(TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	-del $(OBJS) $(TARGET) 2>nul || rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run