ALL=trade trade_eva tradenolock

ALL: Trade.c Trade_eva.c TradeNoLock.c
	gcc -g -Wall -o tradenolock TradeNoLock.c -lpthread	
	gcc -g -Wall -o trade Trade.c -lpthread	
	gcc -g -Wall -o trade_eva Trade_eva.c -lpthread	
clean:
	rm -f *.o trade trade_eva tradenolock
