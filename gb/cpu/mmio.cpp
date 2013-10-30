#ifdef CPU_CPP
#include <ncursesw/curses.h>
#include <sys/time.h>

unsigned CPU::wram_addr(uint16 addr) const {
  addr &= 0x1fff;
  if(addr < 0x1000) return addr;
  auto bank = status.wram_bank + (status.wram_bank == 0);
  return (bank * 0x1000) + (addr & 0x0fff);
}

void CPU::mmio_joyp_poll() {
  static unsigned button = 0;
  static unsigned dpad = 0;
  static time_t prev_sec = 0;
  static suseconds_t prev_usec = 0;

  //TODO:  Move input to an appropriate method in interface
  struct timeval tv;
  int ch;

  gettimeofday(&tv, NULL);
  ch = getch();

  if (ch != ERR) {
    if (tv.tv_sec - prev_sec > 0L || tv.tv_usec - prev_usec > 16666L) {
      prev_sec = tv.tv_sec;
      prev_usec = tv.tv_usec;

      switch(ch) {
      case 'f':
        dpad = 1 << 0;
        break;
      case 's':
        dpad = 1 << 1;
        break;
      case 'e':
        dpad = 1 << 2;
        break;
      case 'd':
        dpad = 1 << 3;
        break;
      case 'j':
        button = 1 << 0;
        break;
      case 'k':
        button = 1 << 1;
        break;
      case ' ':
        button = 1 << 2;
        break;
      case 10:
        button = 1 << 3;
        break;
      }
      //TODO: handle exit key sequence here instead of in ethos
    }
  } else if (tv.tv_sec - prev_sec > 0L || tv.tv_usec - prev_usec > 99999L) {
    button = 0;
    dpad = 0;
  }

  /*
  interface->inputPoll(0, 0, (unsigned)Input::Start) << 3;
  interface->inputPoll(0, 0, (unsigned)Input::Select) << 2;
  interface->inputPoll(0, 0, (unsigned)Input::B) << 1;
  interface->inputPoll(0, 0, (unsigned)Input::A) << 0;

  interface->inputPoll(0, 0, (unsigned)Input::Down) << 3;
  interface->inputPoll(0, 0, (unsigned)Input::Up) << 2;
  interface->inputPoll(0, 0, (unsigned)Input::Left) << 1;
  interface->inputPoll(0, 0, (unsigned)Input::Right) << 0;
  */

  status.joyp = 0x0f;
  if(status.p15 == 1 && status.p14 == 1) status.joyp -= status.mlt_req;
  if(status.p15 == 0) status.joyp &= button ^ 0x0f;
  if(status.p14 == 0) status.joyp &= dpad ^ 0x0f;
  if(status.joyp != 0x0f) interrupt_raise(Interrupt::Joypad);
}

uint8 CPU::mmio_read(uint16 addr) {
  if(addr >= 0xc000 && addr <= 0xfdff) return wram[wram_addr(addr)];
  if(addr >= 0xff80 && addr <= 0xfffe) return hram[addr & 0x7f];

  if(addr == 0xff00) {  //JOYP
    return (status.p15 << 5)
         | (status.p14 << 4)
         | (status.joyp << 0);
  }

  if(addr == 0xff01) {  //SB
    return 0xff;
  }

  if(addr == 0xff02) {  //SC
    return (status.serial_transfer << 7)
         | (status.serial_clock << 0);
  }

  if(addr == 0xff04) {  //DIV
    return status.div;
  }

  if(addr == 0xff05) {  //TIMA
    return status.tima;
  }

  if(addr == 0xff06) {  //TMA
    return status.tma;
  }

  if(addr == 0xff07) {  //TAC
    return (status.timer_enable << 2)
         | (status.timer_clock << 0);
  }

  if(addr == 0xff0f) {  //IF
    return (status.interrupt_request_joypad << 4)
         | (status.interrupt_request_serial << 3)
         | (status.interrupt_request_timer << 2)
         | (status.interrupt_request_stat << 1)
         | (status.interrupt_request_vblank << 0);
  }

  if(addr == 0xff4d) {  //KEY1
    return (status.speed_double << 7);
  }

  if(addr == 0xff55) {  //HDMA5
    return (status.dma_length / 16) - 1;
  }

  if(addr == 0xff56) {  //RP
    return 0x02;
  }

  if(addr == 0xff6c) {  //???
    return 0xfe | status.ff6c;
  }

  if(addr == 0xff70) {  //SVBK
    return status.wram_bank;
  }

  if(addr == 0xff72) {  //???
    return status.ff72;
  }

  if(addr == 0xff73) {  //???
    return status.ff73;
  }

  if(addr == 0xff74) {  //???
    return status.ff74;
  }

  if(addr == 0xff75) {  //???
    return 0x8f | status.ff75;
  }

  if(addr == 0xff76) {  //???
    return 0x00;
  }

  if(addr == 0xff77) {  //???
    return 0x00;
  }

  if(addr == 0xffff) {  //IE
    return (status.interrupt_enable_joypad << 4)
         | (status.interrupt_enable_serial << 3)
         | (status.interrupt_enable_timer << 2)
         | (status.interrupt_enable_stat << 1)
         | (status.interrupt_enable_vblank << 0);
  }

  return 0x00;
}

void CPU::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0xc000 && addr <= 0xfdff) { wram[wram_addr(addr)] = data; return; }
  if(addr >= 0xff80 && addr <= 0xfffe) { hram[addr & 0x7f] = data; return; }

  if(addr == 0xff00) {  //JOYP
    status.p15 = data & 0x20;
    status.p14 = data & 0x10;
    interface->joypWrite(status.p15, status.p14);
    mmio_joyp_poll();
    return;
  }

  if(addr == 0xff01) {  //SB
    status.serial_data = data;
    return;
  }

  if(addr == 0xff02) {  //SC
    status.serial_transfer = data & 0x80;
    status.serial_clock = data & 0x01;
    if(status.serial_transfer) status.serial_bits = 8;
    return;
  }

  if(addr == 0xff04) {  //DIV
    status.div = 0;
    return;
  }

  if(addr == 0xff05) {  //TIMA
    status.tima = data;
    return;
  }

  if(addr == 0xff06) {  //TMA
    status.tma = data;
    return;
  }

  if(addr == 0xff07) {  //TAC
    status.timer_enable = data & 0x04;
    status.timer_clock = data & 0x03;
    return;
  }

  if(addr == 0xff0f) {  //IF
    status.interrupt_request_joypad = data & 0x10;
    status.interrupt_request_serial = data & 0x08;
    status.interrupt_request_timer = data & 0x04;
    status.interrupt_request_stat = data & 0x02;
    status.interrupt_request_vblank = data & 0x01;
    return;
  }

  if(addr == 0xff46) {  //DMA
    for(unsigned n = 0x00; n <= 0x9f; n++) {
      bus.write(0xfe00 + n, bus.read((data << 8) + n));
      add_clocks(4);
    }
    return;
  }

  if(addr == 0xff4d) {  //KEY1
    status.speed_switch = data & 0x01;
    return;
  }

  if(addr == 0xff51) {  //HDMA1
    status.dma_source = (status.dma_source & 0x00ff) | (data << 8);
    return;
  }

  if(addr == 0xff52) {  //HDMA2
    status.dma_source = (status.dma_source & 0xff00) | (data << 0);
    return;
  }

  if(addr == 0xff53) {  //HDMA3
    status.dma_target = (status.dma_target & 0x00ff) | (data << 8);
    return;
  }

  if(addr == 0xff54) {  //HDMA4
    status.dma_target = (status.dma_target & 0xff00) | (data << 0);
    return;
  }

  if(addr == 0xff55) {  //HDMA5
    status.dma_mode = data & 0x80;
    status.dma_length = ((data & 0x7f) + 1) * 16;

    if(status.dma_mode == 0) do {
      bus.write(status.dma_target++, bus.read(status.dma_source++));
      add_clocks(4 << status.speed_double);
    } while(--status.dma_length);
    return;
  }

  if(addr == 0xff56) {  //RP
    return;
  }

  if(addr == 0xff6c) {  //???
    status.ff6c = data & 0x01;
    return;
  }

  if(addr == 0xff72) {  //???
    status.ff72 = data;
    return;
  }

  if(addr == 0xff73) {  //???
    status.ff73 = data;
    return;
  }

  if(addr == 0xff74) {  //???
    status.ff74 = data;
    return;
  }

  if(addr == 0xff75) {  //???
    status.ff75 = data & 0x70;
    return;
  }

  if(addr == 0xff70) {  //SVBK
    status.wram_bank = data & 0x07;
    return;
  }

  if(addr == 0xffff) {  //IE
    status.interrupt_enable_joypad = data & 0x10;
    status.interrupt_enable_serial = data & 0x08;
    status.interrupt_enable_timer = data & 0x04;
    status.interrupt_enable_stat = data & 0x02;
    status.interrupt_enable_vblank = data & 0x01;
    return;
  }
}

#endif
