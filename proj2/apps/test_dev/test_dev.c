#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 4096 
static char receive[BUFFER_LENGTH];

int main(){
   int ret, fd;
   char string_to_send[BUFFER_LENGTH];
   printf("Starting device test code...\n");
   fd = open("/dev/rsvdev", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   /*
   printf("Type in a short string to send to the kernel module:\n");
   scanf("%[^\n]%*c", string_to_send);                // Read in a string (with spaces)
   printf("Writing message to the device [%s].\n", string_to_send);
   ret = write(fd, string_to_send, strlen(string_to_send)); // Send the string to the LKM
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
   printf("Press ENTER to read back from the device...\n");
   getchar();
   */
   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", receive);
   printf("End of the program\n");
   return 0;
}