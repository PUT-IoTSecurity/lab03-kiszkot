main:
	gcc ./main.c ./include/Romulus-M/encrypt.c ./include/Romulus-M/romulus_m_reference.c ./include/Romulus-M/skinny_reference.c -o main -Iinclude -lwiringPi -lwiringPiDev

test:
	gcc ./main_test.c ./include/Romulus-M/encrypt.c ./include/Romulus-M/romulus_m_reference.c ./include/Romulus-M/skinny_reference.c -o main_test -Iinclude -g