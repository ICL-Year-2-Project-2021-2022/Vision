#include <stdio.h>
#include <string.h>
int main ()
{
	char* msg = "Hello message from the FPGA!";
	FILE* fp;
	char prompt = 0;
	int messageCounter = 0;
	fp = fopen ("/dev/uart_0", "r+");
	if (fp) {
		printf("UART detected.\n");
		while (1) {
			char *msgToSend;
			asprintf(&msgToSend, "%s, message id: %d\n", msg, messageCounter);
			printf("FPGA sending\n");
			char response_buffer[64];
			int index = 0;
			fwrite (msgToSend, strlen(msgToSend), 1, fp);
			prompt = getc(fp);
			while(prompt != '\n' && prompt != 0) {
				response_buffer[index] = prompt;
				prompt = getc(fp);
				index++;
			}
			printf("Received: %s\n", response_buffer);
			messageCounter++;
			usleep(100000);
		}
		fclose (fp);
	} else {
		printf("UART not detected.");
	}
 	return 0;
}
