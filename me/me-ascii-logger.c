#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <omp.h>
#include "me.h"

void print_market_order(MeOrder *o, size_t id)
{
	printf("%8u: NEW ORDER (MARKET): SIDE=%s QUANTITY=%lu\n",
		(unsigned int) id,
		o->side == ME_SIDE_BUY ? "BUY" : "SELL",
		o->quantity);
}

void print_limit_order(MeOrder *o, size_t id)
{
	printf("%8u: NEW ORDER (LIMIT): SIDE=%s QUANTITY=%lu PRICE=%lu\n",
		(unsigned int) id,
		o->side == ME_SIDE_BUY ? "BUY" : "SELL",
		o->quantity,
		o->price);
}

void print_message(MeMessage *message)
{
	switch (message->msg_type) {
	case ME_MESSAGE_PANIC:
		fprintf(stderr, "Engine shutdown via panic. Bailing out.\n");
		exit(0);
	case ME_MESSAGE_SET_MARKET_PRICE:
		printf("%8u: SET MARKET PRICE: PRICE=%lu\n",
			(unsigned int) message->security_id,
			(unsigned long) message->message.set_market_price);
		break;
	case ME_MESSAGE_NEW_ORDER:
		if (message->message.order.ord_type == ME_ORDER_MARKET) {
			print_market_order(&message->message.order,
				message->security_id);	
		} else {
			print_limit_order(&message->message.order,
				message->security_id);
		}
		break;
	/* TODO */
        case ME_MESSAGE_CANCEL_ORDER:
        case ME_MESSAGE_TRADE:
        case ME_MESSAGE_ORDER_EXECUTED:
        	break;
        }
}

int main(void)
{
	MeClientContext context;
	MeMessage message;
	if (me_client_init_context(&context) != 0) {
		fprintf(stderr,
			"Could not init client context. Is the engine running?\n");
		fprintf(stderr,
			"Opening queues %s and %s failed ",
			me_in_queue_name,
			me_out_queue_name);
		perror("with");
		return errno;
	}

	while (!me_client_get_message(&context, &message))
		print_message(&message);

	perror("Retriving message failed");

	return 1;
}
