#include <iostream>
#include <string>
#include <sfc/sfc.hpp>



namespace SuperFamicom {

file tracerfp;


CPU cpu;
#include "dma.cpp"
#include "memory.cpp"
#include "io.cpp"
#include "timing.cpp"
#include "irq.cpp"
#include "joypad.cpp"
#include "serialization.cpp"

auto CPU::interruptPending() const -> bool { return status.interruptPending; }
auto CPU::pio() const -> uint8 { return io.pio; }
auto CPU::joylatch() const -> bool { return io.joypadStrobeLatch; }

CPU::CPU() {
  PPUcounter::scanline = {&CPU::scanline, this};
}

auto CPU::Enter() -> void {
  while(true) scheduler.synchronize(), cpu.main();
}

extern int spcWriteOccuredAdr[5];
extern int spcWriteOccured;

auto CPU::main() -> void {

  /*
  if (r.pc == 0xbfad1d
		  || r.pc == 0x09d456
		  //|| r.pc == 0x8095ac Zu oft
		  //|| r.pc == 0x89d49c Zu oft
		  || r.pc == 0x84a123
		  //|| r.pc == 0x809409 Zu oft
		  //|| r.pc == 0x89d71f Zu oft
		  || r.pc == 0x09d3e5
		  || r.pc == 0x09d006
		  || r.pc == 0x09d0ef
		  || r.pc == 0x09d0f2
		  || r.pc == 0x09d0fd
		  || r.pc == 0x09d10d
		  || r.pc == 0x09d110
		  || r.pc == 0x09d11b
		  || r.pc == 0x09d11e
		  || r.pc == 0x09d130
		  || r.pc == 0x09d14a
		  || r.pc == 0x09d164
		  || r.pc == 0x09d16f
		  || r.pc == 0x09d17c
		  || r.pc == 0x09d189
		  || r.pc == 0x09d194
		  || r.pc == 0x09d1ee
		  || r.pc == 0x09d1f1
		  || r.pc == 0x09d1fc
		  || r.pc == 0x09d214
		  || r.pc == 0x09d26e
		  || r.pc == 0x09d271
		  || r.pc == 0x09d27c
		  || r.pc == 0x09d3e5
		  || r.pc == 0x09d3f3
		  || r.pc == 0x09d410
		  || r.pc == 0xbfad1d
		  || r.pc == 0xbfad30
		  || r.pc == 0xbfad43
		  || r.pc == 0xbfad49
		  || r.pc == 0xbfad6c
		  || r.pc == 0xbfb00c

		  )
		  */

  /*
  if (r.pc == 0x808082
		  || r.pc == 0x808085
		  || r.pc == 0x808088
		  || r.pc == 0x80808c
		  || r.pc == 0x808090
		  || r.pc == 0x808a81
		  || r.pc == 0x808e64
		  || r.pc == 0x808eaf
		  || r.pc == 0x808ef8
		  || r.pc == 0x808efb
		  || r.pc == 0x8097e8
		  || r.pc == 0x809812
		  || r.pc == 0x80d134
		  || r.pc == 0x80d142
		  || r.pc == 0x8184e9
		  || r.pc == 0x8184ed
		  || r.pc == 0x81acb3
		  || r.pc == 0x81acc5
		  || r.pc == 0x81ba04
		  || r.pc == 0x81ba07
		  || r.pc == 0x84839c
		  || r.pc == 0x84a09c
		  || r.pc == 0x89d0ef
		  || r.pc == 0x89d0f2
		  || r.pc == 0x89d0fd
		  || r.pc == 0x89d10d
		  || r.pc == 0x89d110
		  || r.pc == 0x89d11b
		  || r.pc == 0x89d11e
		  || r.pc == 0x89d130
		  || r.pc == 0x89d14a
		  || r.pc == 0x89d164
		  || r.pc == 0x89d16f
		  || r.pc == 0x89d17c
		  || r.pc == 0x89d189
		  || r.pc == 0x89d194
		  || r.pc == 0x89d1ee
		  || r.pc == 0x89d1f1
		  || r.pc == 0x89d1fc
		  || r.pc == 0x89d214
		  || r.pc == 0x89d26e
		  || r.pc == 0x89d271
		  || r.pc == 0x89d27c)
  {
	  std::cout<< disassemble()<<std::endl;

  }
  */

	/*
  if (tracerfp)
  {
	  string disassembled = disassemble();
	  if (disassembled[7] == 'j' && disassembled[8] == 's')
	  {
		  tracerfp.print(disassembled, "\n");

		  //std::cout<< disassembled<<std::endl;
	  }
	  else if (disassembled[7] == 'r' && disassembled[8] == 't')
	  {
		  tracerfp.print(disassembled, "\n");
	  }
  }
*/
  if (tracerfp)
  {
	  string disassembled = disassemble();
	  tracerfp.print(disassembled, "\n");
  }

  if (spcWriteOccured>1)
  {
	  if (spcWriteOccuredAdr[0]==0x2140)
	  {
		  printf("Unallowed SPC Operation! %x %x %x ***********\n",
				  spcWriteOccuredAdr[0],
				  spcWriteOccuredAdr[1],
				  spcWriteOccuredAdr[2]);

		  if (tracerfp)
			{
			  string disassembled = disassemble();
			  tracerfp.print("Unallowed SPC", "\n");
			}

	  }
  }

  spcWriteOccured=0;

  if (r.pc == 0x89d14a)
  {
    printf("Transmission\n",r.a.w);
  }

  /*
  if (r.pc == 0x89d74c)
  {

    printf("Transmission finished\n",r.a.w);
  }
  */
  if (r.pc == 0x81ba53)
  {

    printf("Ausgeführt\n",r.a.w);
  }


  if (r.pc == 0x89d012)
  {
	  std::cout<< disassemble()<<std::endl;
  }




  /*
  if (r.pc == 0x8095ac)
  {
	  if (tracerfp)
	  {
		  char buf[512];
		  sprintf(buf,"Volume set to %x?\n",r.a.w);
	  	  tracerfp.print(buf, "\n");
	  }

	  printf("Volume set to %x?\n",r.a.w);
  }
  */

  uint24 fileAddr=((r.pc & 0x7f0000) >> 1) + (r.pc & 0x7fff);

  //if (r.pc == 0x09d00c || r.pc== 0xbfad43)
  if (fileAddr == 0x4d00c || fileAddr == 0x1fad43)
  {
	  if (tracerfp)
	  {
		  char buf[512];
		  sprintf(buf,"Soundbase set to %x?\n",r.a.w);
	  	  tracerfp.print(buf, "\n");
	  }

	  printf("Soundbase set to %x?\n",r.a.w);
  }

  //if (r.pc == 0x09d0ef || r.pc== 0xd388)
  /*
  if (fileAddr == 0x4d0ef || fileAddr == 0x5388)
  {
	  if (tracerfp)
	  {
		  char buf[512];
		  sprintf(buf,"Global Track set to %x?\n",r.a.w);
	  	  tracerfp.print(buf, "\n");
	  }

	  printf("Global Track to %x?\n",r.a.w);
  }
  */

  if (fileAddr == 0x04d012)
  {
	  std::cout<< disassemble()<<std::endl;
  }

	if (fileAddr == 0x04d7ab)
	{
		std::cout<< "Play soundeffect start"<<std::endl;
	}
	if (fileAddr == 0x04d756)
	{
		std::cout<< "Play soundeffect end"<<std::endl;
	}


  /*
  if (r.pc == 0x09d3e5)
  {
	  printf("Select Track in Soundbase %d\n",r.x.w);
  }
  */


  if(status.interruptPending) {
    status.interruptPending = false;
    if(status.nmiPending) {
      status.nmiPending = false;
      r.vector = r.e ? 0xfffa : 0xffea;
      interrupt();
      debug(cpu.nmi);
    } else if(status.irqPending) {
      status.irqPending = false;
      r.vector = r.e ? 0xfffe : 0xffee;
      interrupt();
      debug(cpu.irq);
    } else if(status.resetPending) {
      status.resetPending = false;
      step(132);
      r.vector = 0xfffc;
      interrupt();
    } else if(status.powerPending) {
      status.powerPending = false;
      step(186);
      r.pc.l = bus.read(0xfffc, r.mdr);
      r.pc.h = bus.read(0xfffd, r.mdr);
    }
  }

  debug(cpu.execute, r.pc);
  instruction();
}

auto CPU::load(Markup::Node node) -> bool {
  version = max(1, min(2, node["cpu/version"].natural()));
  return true;
}

auto CPU::power() -> void {
  for(auto& byte : wram) byte = random(0x55);

  //CPU
  r.a = 0x0000;
  r.x = 0x0000;
  r.y = 0x0000;
  r.s = 0x01ff;

  //DMA
  for(auto& channel : this->channel) {
    channel.direction = 1;
    channel.indirect = true;
    channel.unused = true;
    channel.reverseTransfer = true;
    channel.fixedTransfer = true;
    channel.transferMode = 7;

    channel.targetAddress = 0xff;

    channel.sourceAddress = 0xffff;
    channel.sourceBank = 0xff;

    channel.transferSize = 0xffff;
    channel.indirectBank = 0xff;

    channel.hdmaAddress = 0xffff;
    channel.lineCounter = 0xff;
    channel.unknown = 0xff;
  }

  status.powerPending = true;
  status.interruptPending = true;
}

auto CPU::reset() -> void {
  create(Enter, system.colorburst() * 6.0);
  coprocessors.reset();
  PPUcounter::reset();

  function<auto (uint24, uint8) -> uint8> reader;
  function<auto (uint24, uint8) -> void> writer;

  reader = {&CPU::readAPU, this};
  writer = {&CPU::writeAPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2140-217f");

  reader = {&CPU::readCPU, this};
  writer = {&CPU::writeCPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2180-2183,4016-4017,4200-421f");

  reader = {&CPU::readDMA, this};
  writer = {&CPU::writeDMA, this};
  bus.map(reader, writer, "00-3f,80-bf:4300-437f");

  reader = [](uint24 addr, uint8) -> uint8 { return cpu.wram[addr]; };
  writer = [](uint24 addr, uint8 data) -> void { cpu.wram[addr] = data; };
  bus.map(reader, writer, "00-3f,80-bf:0000-1fff", 0x2000);
  bus.map(reader, writer, "7e-7f:0000-ffff", 0x20000);

  //CPU
  r.pc     = 0x000000;
  r.x.h    = 0x00;
  r.y.h    = 0x00;
  r.s.h    = 0x01;
  r.d      = 0x0000;
  r.db     = 0x00;
  r.p      = 0x34;
  r.e      = 1;
  r.mdr    = 0x00;
  r.wai    = false;
  r.vector = 0xfffc;  //reset vector address

  //$2140-217f
  for(auto& port : io.port) port = 0x00;

  //$2181-$2183
  io.wramAddress = 0x000000;

  //$4016-$4017
  io.joypadStrobeLatch = 0;

  //$4200
  io.nmiEnabled = false;
  io.hirqEnabled = false;
  io.virqEnabled = false;
  io.autoJoypadPoll = false;

  //$4201
  io.pio = 0xff;

  //$4202-$4203
  io.wrmpya = 0xff;
  io.wrmpyb = 0xff;

  //$4204-$4206
  io.wrdiva = 0xffff;
  io.wrdivb = 0xff;

  //$4207-$420a
  io.hirqPos = 0x01ff;
  io.virqPos = 0x01ff;

  //$420d
  io.romSpeed = 8;

  //$4214-$4217
  io.rddiv = 0x0000;
  io.rdmpy = 0x0000;

  //$4218-$421f
  io.joy1 = 0x0000;
  io.joy2 = 0x0000;
  io.joy3 = 0x0000;
  io.joy4 = 0x0000;

  //ALU
  alu.mpyctr = 0;
  alu.divctr = 0;
  alu.shift = 0;

  //DMA
  for(auto& channel : this->channel) {
    channel.dmaEnabled = false;
    channel.hdmaEnabled = false;

    channel.hdmaCompleted = false;
    channel.hdmaDoTransfer = false;
  }

  pipe.valid = false;
  pipe.addr = 0;
  pipe.data = 0;

  //Timing
  status.clockCount = 0;
  status.lineClocks = lineclocks();

  status.irqLock = false;
  status.dramRefreshPosition = (version == 1 ? 530 : 538);
  status.dramRefreshed = false;

  status.hdmaInitPosition = (version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
  status.hdmaInitTriggered = false;

  status.hdmaPosition = 1104;
  status.hdmaTriggered = false;

  status.nmiValid      = false;
  status.nmiLine       = false;
  status.nmiTransition = false;
  status.nmiPending    = false;
  status.nmiHold       = false;

  status.irqValid      = false;
  status.irqLine       = false;
  status.irqTransition = false;
  status.irqPending    = false;
  status.irqHold       = false;

  status.resetPending = !status.powerPending;
  status.interruptPending = true;

  status.dmaActive   = false;
  status.dmaCounter  = 0;
  status.dmaClocks   = 0;
  status.dmaPending  = false;
  status.hdmaPending = false;
  status.hdmaMode    = 0;

  status.autoJoypadActive  = false;
  status.autoJoypadLatch   = false;
  status.autoJoypadCounter = 0;
  status.autoJoypadClock   = 0;
}

}
