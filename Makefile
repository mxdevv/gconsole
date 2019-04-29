COMP = g++
FILE_MAIN_TEST = test
FILE_OUT_LIB = gconsole
LIBS = -lncurses
COMP_KEYS = #-fpermissive

test:
	$(COMP) ./$(FILE_MAIN_TEST).cpp -o ./$(FILE_MAIN_TEST) $(LIBS) $(COMP_KEYS)
