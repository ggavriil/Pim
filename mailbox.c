#include "mailbox.h"
#include "utils.h"
#include "uart.h"

/* some code adapted from https://github.com/dwelch67/raspberrypi/blob/master/video01/ */

unsigned int mailbox_write (unsigned int fbinfo_addr, unsigned int channel) {
  unsigned int mailbox;

  mailbox=0x2000B880;
  while(1) {
    if((GET32(mailbox+0x18)&0x80000000)==0) break;
  }
  PUT32(mailbox+0x20,fbinfo_addr+channel);
  return(0);
}

unsigned int mailbox_read (unsigned int channel) {
  unsigned int ra;
  unsigned int mailbox;

  mailbox=0x2000B880;
  while(1) {
    while(1) {
      ra=GET32(mailbox+0x18);
      if((ra&0x40000000)==0) break;
    }
    //hexstrings(ra);
    ra=GET32(mailbox+0x00);
    //hexstring(ra);
    if((ra&0xF)==channel) break;
  }
  return(ra);
}
