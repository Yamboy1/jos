#include "stdlib.h"

#include "jbfifo.cc" 
#include "jjidtent.h"
#include "jj386int.h"

#include "jjmachin.h"

/*
 * Protected-mode interrupt descriptor table (a.k.a., IDT).
 */

/* $ When these work, make them instance variables. */

class IDT_register_contents
{
 public:
  void setIDT(const jjIDT *idtp);
  void setIDTRegisterContents(void);

 private:
#ifdef NOTDEF
  jju8 my6ByteMemoryArea[6];
#else
  jju16 my6ByteMemoryArea[3];
#endif
};

void IDT_register_contents::setIDT(const jjIDT *idtp)
{
  my6ByteMemoryArea[0] = sizeof(*idtp);

  jju32 idtp_bits = (jju32)idtp;
  my6ByteMemoryArea[1] = (idtp_bits & 0xffff);
  my6ByteMemoryArea[2] = (idtp_bits >> 16) & 0xffff;
}

void IDT_register_contents::setIDTRegisterContents(void)
{
#ifdef NOTDEF
  dumpmem(this, sizeof(*this));
#endif
  asm("lidt idt_register_contents"); // $$$$$ WORKS OFF GLOBAL SO assembler can get address.
}

/* $$$$$$$$$ These should not be globals. */
IDT_register_contents idt_register_contents; // Used below.
jjIDT idt;

/*
 * jj386Interrupts
 */

jj386Interrupts::jj386Interrupts()
  : jbMachine()
{
  /* Not much to do now. */
  ;
}

void jj386Interrupts::enableInterrupts(void)
{
  asm("sti");
}

void jj386Interrupts::disableInterrupts(void)
{
  asm("cli");
}

void jj386Interrupts::assignInterrupts(void)
{
  buildIDT();

  idt_register_contents.setIDT(&idt);
  idt_register_contents.setIDTRegisterContents();
}


jjBoolean jj386Interrupts::interruptFIFOEmpty(void)
{
  disableInterrupts();

  jjBoolean retval = myRecordedInterrupts.isEmpty();

  enableInterrupts();

  return(retval);
}

jju16 jj386Interrupts::popOldestInterruptNumber(void)
{
  disableInterrupts();

  jju16 retval = myRecordedInterrupts.pop();

  enableInterrupts();

  return(retval);
}


int asm_debug = 1;

/*
 * This cast in here is a Sign From God that this is not
 * partitioned correctly.
 */

#define HANDLER(n) \
    if (asm_debug && (n != 8) && (n != 9) ) kprintf("<%d> ", n);\
    ((jjMachine *)jbmp)->myRecordedInterrupts.push(n);

void jj386Interrupts::interrupt000Handler(void) { HANDLER(0) }
void jj386Interrupts::interrupt001Handler(void) { HANDLER(1) }
void jj386Interrupts::interrupt002Handler(void) { HANDLER(2) }
void jj386Interrupts::interrupt003Handler(void) { HANDLER(3) }
void jj386Interrupts::interrupt004Handler(void) { HANDLER(4) }
void jj386Interrupts::interrupt005Handler(void) { HANDLER(5) }
void jj386Interrupts::interrupt006Handler(void) { HANDLER(6) }
void jj386Interrupts::interrupt007Handler(void) { HANDLER(7) }
#ifdef HANDLE_CLOCK
void jj386Interrupts::interrupt008Handler(void) { HANDLER(8) }
#else
void jj386Interrupts::interrupt008Handler(void) { }
#endif
void jj386Interrupts::interrupt009Handler(void) { HANDLER(9) }

void jj386Interrupts::interrupt010Handler(void) { HANDLER(10) }
void jj386Interrupts::interrupt011Handler(void) { HANDLER(11) }
void jj386Interrupts::interrupt012Handler(void) { HANDLER(12) }
void jj386Interrupts::interrupt013Handler(void) { HANDLER(13) }
void jj386Interrupts::interrupt014Handler(void) { HANDLER(14) }
void jj386Interrupts::interrupt015Handler(void) { HANDLER(15) }
void jj386Interrupts::interrupt016Handler(void) { HANDLER(16) }
void jj386Interrupts::interrupt017Handler(void) { HANDLER(17) }
void jj386Interrupts::interrupt018Handler(void) { HANDLER(18) }
void jj386Interrupts::interrupt019Handler(void) { HANDLER(19) }

void jj386Interrupts::interrupt020Handler(void) { HANDLER(20) }
void jj386Interrupts::interrupt021Handler(void) { HANDLER(21) }
void jj386Interrupts::interrupt022Handler(void) { HANDLER(22) }
void jj386Interrupts::interrupt023Handler(void) { HANDLER(23) }
void jj386Interrupts::interrupt024Handler(void) { HANDLER(24) }
void jj386Interrupts::interrupt025Handler(void) { HANDLER(25) }
void jj386Interrupts::interrupt026Handler(void) { HANDLER(26) }
void jj386Interrupts::interrupt027Handler(void) { HANDLER(27) }
void jj386Interrupts::interrupt028Handler(void) { HANDLER(28) }
void jj386Interrupts::interrupt029Handler(void) { HANDLER(29) }

void jj386Interrupts::interrupt030Handler(void) { HANDLER(30) }
void jj386Interrupts::interrupt031Handler(void) { HANDLER(31) }
void jj386Interrupts::interrupt032Handler(void) { HANDLER(32) }
void jj386Interrupts::interrupt033Handler(void) { HANDLER(33) }
void jj386Interrupts::interrupt034Handler(void) { HANDLER(34) }
void jj386Interrupts::interrupt035Handler(void) { HANDLER(35) }
void jj386Interrupts::interrupt036Handler(void) { HANDLER(36) }
void jj386Interrupts::interrupt037Handler(void) { HANDLER(37) }
void jj386Interrupts::interrupt038Handler(void) { HANDLER(38) }
void jj386Interrupts::interrupt039Handler(void) { HANDLER(39) }

void jj386Interrupts::interrupt040Handler(void) { HANDLER(40) }
void jj386Interrupts::interrupt041Handler(void) { HANDLER(41) }
void jj386Interrupts::interrupt042Handler(void) { HANDLER(42) }
void jj386Interrupts::interrupt043Handler(void) { HANDLER(43) }
void jj386Interrupts::interrupt044Handler(void) { HANDLER(44) }
void jj386Interrupts::interrupt045Handler(void) { HANDLER(45) }
void jj386Interrupts::interrupt046Handler(void) { HANDLER(46) }
void jj386Interrupts::interrupt047Handler(void) { HANDLER(47) }
void jj386Interrupts::interrupt048Handler(void) { HANDLER(48) }
void jj386Interrupts::interrupt049Handler(void) { HANDLER(49) }

void jj386Interrupts::interrupt050Handler(void) { HANDLER(50) }
void jj386Interrupts::interrupt051Handler(void) { HANDLER(51) }
void jj386Interrupts::interrupt052Handler(void) { HANDLER(52) }
void jj386Interrupts::interrupt053Handler(void) { HANDLER(53) }
void jj386Interrupts::interrupt054Handler(void) { HANDLER(54) }
void jj386Interrupts::interrupt055Handler(void) { HANDLER(55) }
void jj386Interrupts::interrupt056Handler(void) { HANDLER(56) }
void jj386Interrupts::interrupt057Handler(void) { HANDLER(57) }
void jj386Interrupts::interrupt058Handler(void) { HANDLER(58) }
void jj386Interrupts::interrupt059Handler(void) { HANDLER(59) }

void jj386Interrupts::interrupt060Handler(void) { HANDLER(60) }
void jj386Interrupts::interrupt061Handler(void) { HANDLER(61) }
void jj386Interrupts::interrupt062Handler(void) { HANDLER(62) }
void jj386Interrupts::interrupt063Handler(void) { HANDLER(63) }
void jj386Interrupts::interrupt064Handler(void) { HANDLER(64) }
void jj386Interrupts::interrupt065Handler(void) { HANDLER(65) }
void jj386Interrupts::interrupt066Handler(void) { HANDLER(66) }
void jj386Interrupts::interrupt067Handler(void) { HANDLER(67) }
void jj386Interrupts::interrupt068Handler(void) { HANDLER(68) }
void jj386Interrupts::interrupt069Handler(void) { HANDLER(69) }

void jj386Interrupts::interrupt070Handler(void) { HANDLER(70) }
void jj386Interrupts::interrupt071Handler(void) { HANDLER(71) }
void jj386Interrupts::interrupt072Handler(void) { HANDLER(72) }
void jj386Interrupts::interrupt073Handler(void) { HANDLER(73) }
void jj386Interrupts::interrupt074Handler(void) { HANDLER(74) }
void jj386Interrupts::interrupt075Handler(void) { HANDLER(75) }
void jj386Interrupts::interrupt076Handler(void) { HANDLER(76) }
void jj386Interrupts::interrupt077Handler(void) { HANDLER(77) }
void jj386Interrupts::interrupt078Handler(void) { HANDLER(78) }
void jj386Interrupts::interrupt079Handler(void) { HANDLER(79) }

void jj386Interrupts::interrupt080Handler(void) { HANDLER(80) }
void jj386Interrupts::interrupt081Handler(void) { HANDLER(81) }
void jj386Interrupts::interrupt082Handler(void) { HANDLER(82) }
void jj386Interrupts::interrupt083Handler(void) { HANDLER(83) }
void jj386Interrupts::interrupt084Handler(void) { HANDLER(84) }
void jj386Interrupts::interrupt085Handler(void) { HANDLER(85) }
void jj386Interrupts::interrupt086Handler(void) { HANDLER(86) }
void jj386Interrupts::interrupt087Handler(void) { HANDLER(87) }
void jj386Interrupts::interrupt088Handler(void) { HANDLER(88) }
void jj386Interrupts::interrupt089Handler(void) { HANDLER(89) }

void jj386Interrupts::interrupt090Handler(void) { HANDLER(90) }
void jj386Interrupts::interrupt091Handler(void) { HANDLER(91) }
void jj386Interrupts::interrupt092Handler(void) { HANDLER(92) }
void jj386Interrupts::interrupt093Handler(void) { HANDLER(93) }
void jj386Interrupts::interrupt094Handler(void) { HANDLER(94) }
void jj386Interrupts::interrupt095Handler(void) { HANDLER(95) }
void jj386Interrupts::interrupt096Handler(void) { HANDLER(96) }
void jj386Interrupts::interrupt097Handler(void) { HANDLER(97) }
void jj386Interrupts::interrupt098Handler(void) { HANDLER(98) }
void jj386Interrupts::interrupt099Handler(void) { HANDLER(99) }

void jj386Interrupts::interrupt100Handler(void) { HANDLER(100) }
void jj386Interrupts::interrupt101Handler(void) { HANDLER(101) }
void jj386Interrupts::interrupt102Handler(void) { HANDLER(102) }
void jj386Interrupts::interrupt103Handler(void) { HANDLER(103) }
void jj386Interrupts::interrupt104Handler(void) { HANDLER(104) }
void jj386Interrupts::interrupt105Handler(void) { HANDLER(105) }
void jj386Interrupts::interrupt106Handler(void) { HANDLER(106) }
void jj386Interrupts::interrupt107Handler(void) { HANDLER(107) }
void jj386Interrupts::interrupt108Handler(void) { HANDLER(108) }
void jj386Interrupts::interrupt109Handler(void) { HANDLER(109) }

void jj386Interrupts::interrupt110Handler(void) { HANDLER(110) }
void jj386Interrupts::interrupt111Handler(void) { HANDLER(111) }
void jj386Interrupts::interrupt112Handler(void) { HANDLER(112) }
void jj386Interrupts::interrupt113Handler(void) { HANDLER(113) }
void jj386Interrupts::interrupt114Handler(void) { HANDLER(114) }
void jj386Interrupts::interrupt115Handler(void) { HANDLER(115) }
void jj386Interrupts::interrupt116Handler(void) { HANDLER(116) }
void jj386Interrupts::interrupt117Handler(void) { HANDLER(117) }
void jj386Interrupts::interrupt118Handler(void) { HANDLER(118) }
void jj386Interrupts::interrupt119Handler(void) { HANDLER(119) }

void jj386Interrupts::interrupt120Handler(void) { HANDLER(120) }
void jj386Interrupts::interrupt121Handler(void) { HANDLER(121) }
void jj386Interrupts::interrupt122Handler(void) { HANDLER(122) }
void jj386Interrupts::interrupt123Handler(void) { HANDLER(123) }
void jj386Interrupts::interrupt124Handler(void) { HANDLER(124) }
void jj386Interrupts::interrupt125Handler(void) { HANDLER(125) }
void jj386Interrupts::interrupt126Handler(void) { HANDLER(126) }
void jj386Interrupts::interrupt127Handler(void) { HANDLER(127) }
void jj386Interrupts::interrupt128Handler(void) { HANDLER(128) }
void jj386Interrupts::interrupt129Handler(void) { HANDLER(129) }

void jj386Interrupts::interrupt130Handler(void) { HANDLER(130) }
void jj386Interrupts::interrupt131Handler(void) { HANDLER(131) }
void jj386Interrupts::interrupt132Handler(void) { HANDLER(132) }
void jj386Interrupts::interrupt133Handler(void) { HANDLER(133) }
void jj386Interrupts::interrupt134Handler(void) { HANDLER(134) }
void jj386Interrupts::interrupt135Handler(void) { HANDLER(135) }
void jj386Interrupts::interrupt136Handler(void) { HANDLER(136) }
void jj386Interrupts::interrupt137Handler(void) { HANDLER(137) } 
void jj386Interrupts::interrupt138Handler(void) { HANDLER(138) }
void jj386Interrupts::interrupt139Handler(void) { HANDLER(139) } 

void jj386Interrupts::interrupt140Handler(void) { HANDLER(140) }
void jj386Interrupts::interrupt141Handler(void) { HANDLER(141) }
void jj386Interrupts::interrupt142Handler(void) { HANDLER(142) }
void jj386Interrupts::interrupt143Handler(void) { HANDLER(143) }
void jj386Interrupts::interrupt144Handler(void) { HANDLER(144) }
void jj386Interrupts::interrupt145Handler(void) { HANDLER(145) }
void jj386Interrupts::interrupt146Handler(void) { HANDLER(146) }
void jj386Interrupts::interrupt147Handler(void) { HANDLER(147) }
void jj386Interrupts::interrupt148Handler(void) { HANDLER(148) }
void jj386Interrupts::interrupt149Handler(void) { HANDLER(149) }

void jj386Interrupts::interrupt150Handler(void) { HANDLER(150) }
void jj386Interrupts::interrupt151Handler(void) { HANDLER(151) }
void jj386Interrupts::interrupt152Handler(void) { HANDLER(152) }
void jj386Interrupts::interrupt153Handler(void) { HANDLER(153) }
void jj386Interrupts::interrupt154Handler(void) { HANDLER(154) }
void jj386Interrupts::interrupt155Handler(void) { HANDLER(155) }
void jj386Interrupts::interrupt156Handler(void) { HANDLER(156) }
void jj386Interrupts::interrupt157Handler(void) { HANDLER(157) }
void jj386Interrupts::interrupt158Handler(void) { HANDLER(158) }
void jj386Interrupts::interrupt159Handler(void) { HANDLER(159) }

void jj386Interrupts::interrupt160Handler(void) { HANDLER(160) }
void jj386Interrupts::interrupt161Handler(void) { HANDLER(161) }
void jj386Interrupts::interrupt162Handler(void) { HANDLER(162) }
void jj386Interrupts::interrupt163Handler(void) { HANDLER(163) }
void jj386Interrupts::interrupt164Handler(void) { HANDLER(164) }
void jj386Interrupts::interrupt165Handler(void) { HANDLER(165) }
void jj386Interrupts::interrupt166Handler(void) { HANDLER(166) }
void jj386Interrupts::interrupt167Handler(void) { HANDLER(167) }
void jj386Interrupts::interrupt168Handler(void) { HANDLER(168) }
void jj386Interrupts::interrupt169Handler(void) { HANDLER(169) }

void jj386Interrupts::interrupt170Handler(void) { HANDLER(170) }
void jj386Interrupts::interrupt171Handler(void) { HANDLER(171) }
void jj386Interrupts::interrupt172Handler(void) { HANDLER(172) }
void jj386Interrupts::interrupt173Handler(void) { HANDLER(173) }
void jj386Interrupts::interrupt174Handler(void) { HANDLER(174) }
void jj386Interrupts::interrupt175Handler(void) { HANDLER(175) }
void jj386Interrupts::interrupt176Handler(void) { HANDLER(176) }
void jj386Interrupts::interrupt177Handler(void) { HANDLER(177) }
void jj386Interrupts::interrupt178Handler(void) { HANDLER(178) }
void jj386Interrupts::interrupt179Handler(void) { HANDLER(179) }

void jj386Interrupts::interrupt180Handler(void) { HANDLER(180) }
void jj386Interrupts::interrupt181Handler(void) { HANDLER(181) }
void jj386Interrupts::interrupt182Handler(void) { HANDLER(182) }
void jj386Interrupts::interrupt183Handler(void) { HANDLER(183) }
void jj386Interrupts::interrupt184Handler(void) { HANDLER(184) }
void jj386Interrupts::interrupt185Handler(void) { HANDLER(185) }
void jj386Interrupts::interrupt186Handler(void) { HANDLER(186) }
void jj386Interrupts::interrupt187Handler(void) { HANDLER(187) }
void jj386Interrupts::interrupt188Handler(void) { HANDLER(188) }
void jj386Interrupts::interrupt189Handler(void) { HANDLER(189) }

void jj386Interrupts::interrupt190Handler(void) { HANDLER(190) }
void jj386Interrupts::interrupt191Handler(void) { HANDLER(191) }
void jj386Interrupts::interrupt192Handler(void) { HANDLER(192) }
void jj386Interrupts::interrupt193Handler(void) { HANDLER(193) }
void jj386Interrupts::interrupt194Handler(void) { HANDLER(194) }
void jj386Interrupts::interrupt195Handler(void) { HANDLER(195) }
void jj386Interrupts::interrupt196Handler(void) { HANDLER(196) }
void jj386Interrupts::interrupt197Handler(void) { HANDLER(197) }
void jj386Interrupts::interrupt198Handler(void) { HANDLER(198) }
void jj386Interrupts::interrupt199Handler(void) { HANDLER(199) }

void jj386Interrupts::interrupt200Handler(void) { HANDLER(200) }
void jj386Interrupts::interrupt201Handler(void) { HANDLER(201) }
void jj386Interrupts::interrupt202Handler(void) { HANDLER(202) }
void jj386Interrupts::interrupt203Handler(void) { HANDLER(203) }
void jj386Interrupts::interrupt204Handler(void) { HANDLER(204) }
void jj386Interrupts::interrupt205Handler(void) { HANDLER(205) }
void jj386Interrupts::interrupt206Handler(void) { HANDLER(206) }
void jj386Interrupts::interrupt207Handler(void) { HANDLER(207) }
void jj386Interrupts::interrupt208Handler(void) { HANDLER(208) }
void jj386Interrupts::interrupt209Handler(void) { HANDLER(209) }

void jj386Interrupts::interrupt210Handler(void) { HANDLER(210) }
void jj386Interrupts::interrupt211Handler(void) { HANDLER(211) }
void jj386Interrupts::interrupt212Handler(void) { HANDLER(212) }
void jj386Interrupts::interrupt213Handler(void) { HANDLER(213) }
void jj386Interrupts::interrupt214Handler(void) { HANDLER(214) }
void jj386Interrupts::interrupt215Handler(void) { HANDLER(215) }
void jj386Interrupts::interrupt216Handler(void) { HANDLER(216) }
void jj386Interrupts::interrupt217Handler(void) { HANDLER(217) }
void jj386Interrupts::interrupt218Handler(void) { HANDLER(218) }
void jj386Interrupts::interrupt219Handler(void) { HANDLER(219) }

void jj386Interrupts::interrupt220Handler(void) { HANDLER(220) }
void jj386Interrupts::interrupt221Handler(void) { HANDLER(221) }
void jj386Interrupts::interrupt222Handler(void) { HANDLER(222) }
void jj386Interrupts::interrupt223Handler(void) { HANDLER(223) }
void jj386Interrupts::interrupt224Handler(void) { HANDLER(224) }
void jj386Interrupts::interrupt225Handler(void) { HANDLER(225) }
void jj386Interrupts::interrupt226Handler(void) { HANDLER(226) }
void jj386Interrupts::interrupt227Handler(void) { HANDLER(227) }
void jj386Interrupts::interrupt228Handler(void) { HANDLER(228) }
void jj386Interrupts::interrupt229Handler(void) { HANDLER(229) }

void jj386Interrupts::interrupt230Handler(void) { HANDLER(230) }
void jj386Interrupts::interrupt231Handler(void) { HANDLER(231) }
void jj386Interrupts::interrupt232Handler(void) { HANDLER(232) }
void jj386Interrupts::interrupt233Handler(void) { HANDLER(233) }
void jj386Interrupts::interrupt234Handler(void) { HANDLER(234) }
void jj386Interrupts::interrupt235Handler(void) { HANDLER(235) }
void jj386Interrupts::interrupt236Handler(void) { HANDLER(236) }
void jj386Interrupts::interrupt237Handler(void) { HANDLER(237) }
void jj386Interrupts::interrupt238Handler(void) { HANDLER(238) }
void jj386Interrupts::interrupt239Handler(void) { HANDLER(239) }

void jj386Interrupts::interrupt240Handler(void) { HANDLER(240) }
void jj386Interrupts::interrupt241Handler(void) { HANDLER(241) }
void jj386Interrupts::interrupt242Handler(void) { HANDLER(242) }
void jj386Interrupts::interrupt243Handler(void) { HANDLER(243) }
void jj386Interrupts::interrupt244Handler(void) { HANDLER(244) }
void jj386Interrupts::interrupt245Handler(void) { HANDLER(245) }
void jj386Interrupts::interrupt246Handler(void) { HANDLER(246) }
void jj386Interrupts::interrupt247Handler(void) { HANDLER(247) }
void jj386Interrupts::interrupt248Handler(void) { HANDLER(248) }
void jj386Interrupts::interrupt249Handler(void) { HANDLER(249) }

void jj386Interrupts::interrupt250Handler(void) { HANDLER(250) }
void jj386Interrupts::interrupt251Handler(void) { HANDLER(251) }
void jj386Interrupts::interrupt252Handler(void) { HANDLER(252) }
void jj386Interrupts::interrupt253Handler(void) { HANDLER(253) }
void jj386Interrupts::interrupt254Handler(void) { HANDLER(254) }
void jj386Interrupts::interrupt255Handler(void) { HANDLER(255) }

#define ASM_INT_HANDLER(asm_symbol) extern "C" { void asm_symbol(void); };

ASM_INT_HANDLER(int000handler);
ASM_INT_HANDLER(int001handler);
ASM_INT_HANDLER(int002handler);
ASM_INT_HANDLER(int003handler);
ASM_INT_HANDLER(int004handler);
ASM_INT_HANDLER(int005handler);
ASM_INT_HANDLER(int006handler);
ASM_INT_HANDLER(int007handler);
ASM_INT_HANDLER(int008handler);
ASM_INT_HANDLER(int009handler);

ASM_INT_HANDLER(int010handler);
ASM_INT_HANDLER(int011handler);
ASM_INT_HANDLER(int012handler);
ASM_INT_HANDLER(int013handler);
ASM_INT_HANDLER(int014handler);
ASM_INT_HANDLER(int015handler);
ASM_INT_HANDLER(int016handler);
ASM_INT_HANDLER(int017handler);
ASM_INT_HANDLER(int018handler);
ASM_INT_HANDLER(int019handler);

ASM_INT_HANDLER(int020handler);
ASM_INT_HANDLER(int021handler);
ASM_INT_HANDLER(int022handler);
ASM_INT_HANDLER(int023handler);
ASM_INT_HANDLER(int024handler);
ASM_INT_HANDLER(int025handler);
ASM_INT_HANDLER(int026handler);
ASM_INT_HANDLER(int027handler);
ASM_INT_HANDLER(int028handler);
ASM_INT_HANDLER(int029handler);

ASM_INT_HANDLER(int030handler);
ASM_INT_HANDLER(int031handler);
ASM_INT_HANDLER(int032handler);
ASM_INT_HANDLER(int033handler);
ASM_INT_HANDLER(int034handler);
ASM_INT_HANDLER(int035handler);
ASM_INT_HANDLER(int036handler);
ASM_INT_HANDLER(int037handler);
ASM_INT_HANDLER(int038handler);
ASM_INT_HANDLER(int039handler);

ASM_INT_HANDLER(int040handler);
ASM_INT_HANDLER(int041handler);
ASM_INT_HANDLER(int042handler);
ASM_INT_HANDLER(int043handler);
ASM_INT_HANDLER(int044handler);
ASM_INT_HANDLER(int045handler);
ASM_INT_HANDLER(int046handler);
ASM_INT_HANDLER(int047handler);
ASM_INT_HANDLER(int048handler);
ASM_INT_HANDLER(int049handler);

ASM_INT_HANDLER(int050handler);
ASM_INT_HANDLER(int051handler);
ASM_INT_HANDLER(int052handler);
ASM_INT_HANDLER(int053handler);
ASM_INT_HANDLER(int054handler);
ASM_INT_HANDLER(int055handler);
ASM_INT_HANDLER(int056handler);
ASM_INT_HANDLER(int057handler);
ASM_INT_HANDLER(int058handler);
ASM_INT_HANDLER(int059handler);

ASM_INT_HANDLER(int060handler);
ASM_INT_HANDLER(int061handler);
ASM_INT_HANDLER(int062handler);
ASM_INT_HANDLER(int063handler);
ASM_INT_HANDLER(int064handler);
ASM_INT_HANDLER(int065handler);
ASM_INT_HANDLER(int066handler);
ASM_INT_HANDLER(int067handler);
ASM_INT_HANDLER(int068handler);
ASM_INT_HANDLER(int069handler);

ASM_INT_HANDLER(int070handler);
ASM_INT_HANDLER(int071handler);
ASM_INT_HANDLER(int072handler);
ASM_INT_HANDLER(int073handler);
ASM_INT_HANDLER(int074handler);
ASM_INT_HANDLER(int075handler);
ASM_INT_HANDLER(int076handler);
ASM_INT_HANDLER(int077handler);
ASM_INT_HANDLER(int078handler);
ASM_INT_HANDLER(int079handler);

ASM_INT_HANDLER(int080handler);
ASM_INT_HANDLER(int081handler);
ASM_INT_HANDLER(int082handler);
ASM_INT_HANDLER(int083handler);
ASM_INT_HANDLER(int084handler);
ASM_INT_HANDLER(int085handler);
ASM_INT_HANDLER(int086handler);
ASM_INT_HANDLER(int087handler);
ASM_INT_HANDLER(int088handler);
ASM_INT_HANDLER(int089handler);

ASM_INT_HANDLER(int090handler);
ASM_INT_HANDLER(int091handler);
ASM_INT_HANDLER(int092handler);
ASM_INT_HANDLER(int093handler);
ASM_INT_HANDLER(int094handler);
ASM_INT_HANDLER(int095handler);
ASM_INT_HANDLER(int096handler);
ASM_INT_HANDLER(int097handler);
ASM_INT_HANDLER(int098handler);
ASM_INT_HANDLER(int099handler);

ASM_INT_HANDLER(int100handler);
ASM_INT_HANDLER(int101handler);
ASM_INT_HANDLER(int102handler);
ASM_INT_HANDLER(int103handler);
ASM_INT_HANDLER(int104handler);
ASM_INT_HANDLER(int105handler);
ASM_INT_HANDLER(int106handler);
ASM_INT_HANDLER(int107handler);
ASM_INT_HANDLER(int108handler);
ASM_INT_HANDLER(int109handler);

ASM_INT_HANDLER(int110handler);
ASM_INT_HANDLER(int111handler);
ASM_INT_HANDLER(int112handler);
ASM_INT_HANDLER(int113handler);
ASM_INT_HANDLER(int114handler);
ASM_INT_HANDLER(int115handler);
ASM_INT_HANDLER(int116handler);
ASM_INT_HANDLER(int117handler);
ASM_INT_HANDLER(int118handler);
ASM_INT_HANDLER(int119handler);

ASM_INT_HANDLER(int120handler);
ASM_INT_HANDLER(int121handler);
ASM_INT_HANDLER(int122handler);
ASM_INT_HANDLER(int123handler);
ASM_INT_HANDLER(int124handler);
ASM_INT_HANDLER(int125handler);
ASM_INT_HANDLER(int126handler);
ASM_INT_HANDLER(int127handler);
ASM_INT_HANDLER(int128handler);
ASM_INT_HANDLER(int129handler);

ASM_INT_HANDLER(int130handler);
ASM_INT_HANDLER(int131handler);
ASM_INT_HANDLER(int132handler);
ASM_INT_HANDLER(int133handler);
ASM_INT_HANDLER(int134handler);
ASM_INT_HANDLER(int135handler);
ASM_INT_HANDLER(int136handler);
ASM_INT_HANDLER(int137handler);
ASM_INT_HANDLER(int138handler);
ASM_INT_HANDLER(int139handler);

ASM_INT_HANDLER(int140handler);
ASM_INT_HANDLER(int141handler);
ASM_INT_HANDLER(int142handler);
ASM_INT_HANDLER(int143handler);
ASM_INT_HANDLER(int144handler);
ASM_INT_HANDLER(int145handler);
ASM_INT_HANDLER(int146handler);
ASM_INT_HANDLER(int147handler);
ASM_INT_HANDLER(int148handler);
ASM_INT_HANDLER(int149handler);

ASM_INT_HANDLER(int150handler);
ASM_INT_HANDLER(int151handler);
ASM_INT_HANDLER(int152handler);
ASM_INT_HANDLER(int153handler);
ASM_INT_HANDLER(int154handler);
ASM_INT_HANDLER(int155handler);
ASM_INT_HANDLER(int156handler);
ASM_INT_HANDLER(int157handler);
ASM_INT_HANDLER(int158handler);
ASM_INT_HANDLER(int159handler);

ASM_INT_HANDLER(int160handler);
ASM_INT_HANDLER(int161handler);
ASM_INT_HANDLER(int162handler);
ASM_INT_HANDLER(int163handler);
ASM_INT_HANDLER(int164handler);
ASM_INT_HANDLER(int165handler);
ASM_INT_HANDLER(int166handler);
ASM_INT_HANDLER(int167handler);
ASM_INT_HANDLER(int168handler);
ASM_INT_HANDLER(int169handler);

ASM_INT_HANDLER(int170handler);
ASM_INT_HANDLER(int171handler);
ASM_INT_HANDLER(int172handler);
ASM_INT_HANDLER(int173handler);
ASM_INT_HANDLER(int174handler);
ASM_INT_HANDLER(int175handler);
ASM_INT_HANDLER(int176handler);
ASM_INT_HANDLER(int177handler);
ASM_INT_HANDLER(int178handler);
ASM_INT_HANDLER(int179handler);

ASM_INT_HANDLER(int180handler);
ASM_INT_HANDLER(int181handler);
ASM_INT_HANDLER(int182handler);
ASM_INT_HANDLER(int183handler);
ASM_INT_HANDLER(int184handler);
ASM_INT_HANDLER(int185handler);
ASM_INT_HANDLER(int186handler);
ASM_INT_HANDLER(int187handler);
ASM_INT_HANDLER(int188handler);
ASM_INT_HANDLER(int189handler);

ASM_INT_HANDLER(int190handler);
ASM_INT_HANDLER(int191handler);
ASM_INT_HANDLER(int192handler);
ASM_INT_HANDLER(int193handler);
ASM_INT_HANDLER(int194handler);
ASM_INT_HANDLER(int195handler);
ASM_INT_HANDLER(int196handler);
ASM_INT_HANDLER(int197handler);
ASM_INT_HANDLER(int198handler);
ASM_INT_HANDLER(int199handler);

ASM_INT_HANDLER(int200handler);
ASM_INT_HANDLER(int201handler);
ASM_INT_HANDLER(int202handler);
ASM_INT_HANDLER(int203handler);
ASM_INT_HANDLER(int204handler);
ASM_INT_HANDLER(int205handler);
ASM_INT_HANDLER(int206handler);
ASM_INT_HANDLER(int207handler);
ASM_INT_HANDLER(int208handler);
ASM_INT_HANDLER(int209handler);

ASM_INT_HANDLER(int210handler);
ASM_INT_HANDLER(int211handler);
ASM_INT_HANDLER(int212handler);
ASM_INT_HANDLER(int213handler);
ASM_INT_HANDLER(int214handler);
ASM_INT_HANDLER(int215handler);
ASM_INT_HANDLER(int216handler);
ASM_INT_HANDLER(int217handler);
ASM_INT_HANDLER(int218handler);
ASM_INT_HANDLER(int219handler);

ASM_INT_HANDLER(int220handler);
ASM_INT_HANDLER(int221handler);
ASM_INT_HANDLER(int222handler);
ASM_INT_HANDLER(int223handler);
ASM_INT_HANDLER(int224handler);
ASM_INT_HANDLER(int225handler);
ASM_INT_HANDLER(int226handler);
ASM_INT_HANDLER(int227handler);
ASM_INT_HANDLER(int228handler);
ASM_INT_HANDLER(int229handler);

ASM_INT_HANDLER(int230handler);
ASM_INT_HANDLER(int231handler);
ASM_INT_HANDLER(int232handler);
ASM_INT_HANDLER(int233handler);
ASM_INT_HANDLER(int234handler);
ASM_INT_HANDLER(int235handler);
ASM_INT_HANDLER(int236handler);
ASM_INT_HANDLER(int237handler);
ASM_INT_HANDLER(int238handler);
ASM_INT_HANDLER(int239handler);

ASM_INT_HANDLER(int240handler);
ASM_INT_HANDLER(int241handler);
ASM_INT_HANDLER(int242handler);
ASM_INT_HANDLER(int243handler);
ASM_INT_HANDLER(int244handler);
ASM_INT_HANDLER(int245handler);
ASM_INT_HANDLER(int246handler);
ASM_INT_HANDLER(int247handler);
ASM_INT_HANDLER(int248handler);
ASM_INT_HANDLER(int249handler);

ASM_INT_HANDLER(int250handler);
ASM_INT_HANDLER(int251handler);
ASM_INT_HANDLER(int252handler);
ASM_INT_HANDLER(int253handler);
ASM_INT_HANDLER(int254handler);
ASM_INT_HANDLER(int255handler);


void jj386Interrupts::buildIDT(void)
{
 idt.myEntry[0].jumpToHere((void *)&int000handler);
 idt.myEntry[1].jumpToHere((void *)&int001handler);
 idt.myEntry[2].jumpToHere((void *)&int002handler);
 idt.myEntry[3].jumpToHere((void *)&int003handler);
 idt.myEntry[4].jumpToHere((void *)&int004handler);
 idt.myEntry[5].jumpToHere((void *)&int005handler);
 idt.myEntry[6].jumpToHere((void *)&int006handler);
 idt.myEntry[7].jumpToHere((void *)&int007handler);
 idt.myEntry[8].jumpToHere((void *)&int008handler);
 idt.myEntry[9].jumpToHere((void *)&int009handler);

 idt.myEntry[10].jumpToHere((void *)&int010handler);
 idt.myEntry[11].jumpToHere((void *)&int011handler);
 idt.myEntry[12].jumpToHere((void *)&int012handler);
 idt.myEntry[13].jumpToHere((void *)&int013handler);
 idt.myEntry[14].jumpToHere((void *)&int014handler);
 idt.myEntry[15].jumpToHere((void *)&int015handler);
 idt.myEntry[16].jumpToHere((void *)&int016handler);
 idt.myEntry[17].jumpToHere((void *)&int017handler);
 idt.myEntry[18].jumpToHere((void *)&int018handler);
 idt.myEntry[19].jumpToHere((void *)&int019handler);

 idt.myEntry[20].jumpToHere((void *)&int020handler);
 idt.myEntry[21].jumpToHere((void *)&int021handler);
 idt.myEntry[22].jumpToHere((void *)&int022handler);
 idt.myEntry[23].jumpToHere((void *)&int023handler);
 idt.myEntry[24].jumpToHere((void *)&int024handler);
 idt.myEntry[25].jumpToHere((void *)&int025handler);
 idt.myEntry[26].jumpToHere((void *)&int026handler);
 idt.myEntry[27].jumpToHere((void *)&int027handler);
 idt.myEntry[28].jumpToHere((void *)&int028handler);
 idt.myEntry[29].jumpToHere((void *)&int029handler);

 idt.myEntry[30].jumpToHere((void *)&int030handler);
 idt.myEntry[31].jumpToHere((void *)&int031handler);
 idt.myEntry[32].jumpToHere((void *)&int032handler);
 idt.myEntry[33].jumpToHere((void *)&int033handler);
 idt.myEntry[34].jumpToHere((void *)&int034handler);
 idt.myEntry[35].jumpToHere((void *)&int035handler);
 idt.myEntry[36].jumpToHere((void *)&int036handler);
 idt.myEntry[37].jumpToHere((void *)&int037handler);
 idt.myEntry[38].jumpToHere((void *)&int038handler);
 idt.myEntry[39].jumpToHere((void *)&int039handler);

 idt.myEntry[40].jumpToHere((void *)&int040handler);
 idt.myEntry[41].jumpToHere((void *)&int041handler);
 idt.myEntry[42].jumpToHere((void *)&int042handler);
 idt.myEntry[43].jumpToHere((void *)&int043handler);
 idt.myEntry[44].jumpToHere((void *)&int044handler);
 idt.myEntry[45].jumpToHere((void *)&int045handler);
 idt.myEntry[46].jumpToHere((void *)&int046handler);
 idt.myEntry[47].jumpToHere((void *)&int047handler);
 idt.myEntry[48].jumpToHere((void *)&int048handler);
 idt.myEntry[49].jumpToHere((void *)&int049handler);

 idt.myEntry[50].jumpToHere((void *)&int050handler);
 idt.myEntry[51].jumpToHere((void *)&int051handler);
 idt.myEntry[52].jumpToHere((void *)&int052handler);
 idt.myEntry[53].jumpToHere((void *)&int053handler);
 idt.myEntry[54].jumpToHere((void *)&int054handler);
 idt.myEntry[55].jumpToHere((void *)&int055handler);
 idt.myEntry[56].jumpToHere((void *)&int056handler);
 idt.myEntry[57].jumpToHere((void *)&int057handler);
 idt.myEntry[58].jumpToHere((void *)&int058handler);
 idt.myEntry[59].jumpToHere((void *)&int059handler);

 idt.myEntry[60].jumpToHere((void *)&int060handler);
 idt.myEntry[61].jumpToHere((void *)&int061handler);
 idt.myEntry[62].jumpToHere((void *)&int062handler);
 idt.myEntry[63].jumpToHere((void *)&int063handler);
 idt.myEntry[64].jumpToHere((void *)&int064handler);
 idt.myEntry[65].jumpToHere((void *)&int065handler);
 idt.myEntry[66].jumpToHere((void *)&int066handler);
 idt.myEntry[67].jumpToHere((void *)&int067handler);
 idt.myEntry[68].jumpToHere((void *)&int068handler);
 idt.myEntry[69].jumpToHere((void *)&int069handler);

 idt.myEntry[70].jumpToHere((void *)&int070handler);
 idt.myEntry[71].jumpToHere((void *)&int071handler);
 idt.myEntry[72].jumpToHere((void *)&int072handler);
 idt.myEntry[73].jumpToHere((void *)&int073handler);
 idt.myEntry[74].jumpToHere((void *)&int074handler);
 idt.myEntry[75].jumpToHere((void *)&int075handler);
 idt.myEntry[76].jumpToHere((void *)&int076handler);
 idt.myEntry[77].jumpToHere((void *)&int077handler);
 idt.myEntry[78].jumpToHere((void *)&int078handler);
 idt.myEntry[79].jumpToHere((void *)&int079handler);

 idt.myEntry[80].jumpToHere((void *)&int080handler);
 idt.myEntry[81].jumpToHere((void *)&int081handler);
 idt.myEntry[82].jumpToHere((void *)&int082handler);
 idt.myEntry[83].jumpToHere((void *)&int083handler);
 idt.myEntry[84].jumpToHere((void *)&int084handler);
 idt.myEntry[85].jumpToHere((void *)&int085handler);
 idt.myEntry[86].jumpToHere((void *)&int086handler);
 idt.myEntry[87].jumpToHere((void *)&int087handler);
 idt.myEntry[88].jumpToHere((void *)&int088handler);
 idt.myEntry[89].jumpToHere((void *)&int089handler);

 idt.myEntry[90].jumpToHere((void *)&int090handler);
 idt.myEntry[91].jumpToHere((void *)&int091handler);
 idt.myEntry[92].jumpToHere((void *)&int092handler);
 idt.myEntry[93].jumpToHere((void *)&int093handler);
 idt.myEntry[94].jumpToHere((void *)&int094handler);
 idt.myEntry[95].jumpToHere((void *)&int095handler);
 idt.myEntry[96].jumpToHere((void *)&int096handler);
 idt.myEntry[97].jumpToHere((void *)&int097handler);
 idt.myEntry[98].jumpToHere((void *)&int098handler);
 idt.myEntry[99].jumpToHere((void *)&int099handler);

 idt.myEntry[100].jumpToHere((void *)&int100handler);
 idt.myEntry[101].jumpToHere((void *)&int101handler);
 idt.myEntry[102].jumpToHere((void *)&int102handler);
 idt.myEntry[103].jumpToHere((void *)&int103handler);
 idt.myEntry[104].jumpToHere((void *)&int104handler);
 idt.myEntry[105].jumpToHere((void *)&int105handler);
 idt.myEntry[106].jumpToHere((void *)&int106handler);
 idt.myEntry[107].jumpToHere((void *)&int107handler);
 idt.myEntry[108].jumpToHere((void *)&int108handler);
 idt.myEntry[109].jumpToHere((void *)&int109handler);

 idt.myEntry[110].jumpToHere((void *)&int110handler);
 idt.myEntry[111].jumpToHere((void *)&int111handler);
 idt.myEntry[112].jumpToHere((void *)&int112handler);
 idt.myEntry[113].jumpToHere((void *)&int113handler);
 idt.myEntry[114].jumpToHere((void *)&int114handler);
 idt.myEntry[115].jumpToHere((void *)&int115handler);
 idt.myEntry[116].jumpToHere((void *)&int116handler);
 idt.myEntry[117].jumpToHere((void *)&int117handler);
 idt.myEntry[118].jumpToHere((void *)&int118handler);
 idt.myEntry[119].jumpToHere((void *)&int119handler);

 idt.myEntry[120].jumpToHere((void *)&int120handler);
 idt.myEntry[121].jumpToHere((void *)&int121handler);
 idt.myEntry[122].jumpToHere((void *)&int122handler);
 idt.myEntry[123].jumpToHere((void *)&int123handler);
 idt.myEntry[124].jumpToHere((void *)&int124handler);
 idt.myEntry[125].jumpToHere((void *)&int125handler);
 idt.myEntry[126].jumpToHere((void *)&int126handler);
 idt.myEntry[127].jumpToHere((void *)&int127handler);
 idt.myEntry[128].jumpToHere((void *)&int128handler);
 idt.myEntry[129].jumpToHere((void *)&int129handler);

 idt.myEntry[130].jumpToHere((void *)&int130handler);
 idt.myEntry[131].jumpToHere((void *)&int131handler);
 idt.myEntry[132].jumpToHere((void *)&int132handler);
 idt.myEntry[133].jumpToHere((void *)&int133handler);
 idt.myEntry[134].jumpToHere((void *)&int134handler);
 idt.myEntry[135].jumpToHere((void *)&int135handler);
 idt.myEntry[136].jumpToHere((void *)&int136handler);
 idt.myEntry[137].jumpToHere((void *)&int137handler);
 idt.myEntry[138].jumpToHere((void *)&int138handler);
 idt.myEntry[139].jumpToHere((void *)&int139handler);

 idt.myEntry[140].jumpToHere((void *)&int140handler);
 idt.myEntry[141].jumpToHere((void *)&int141handler);
 idt.myEntry[142].jumpToHere((void *)&int142handler);
 idt.myEntry[143].jumpToHere((void *)&int143handler);
 idt.myEntry[144].jumpToHere((void *)&int144handler);
 idt.myEntry[145].jumpToHere((void *)&int145handler);
 idt.myEntry[146].jumpToHere((void *)&int146handler);
 idt.myEntry[147].jumpToHere((void *)&int147handler);
 idt.myEntry[148].jumpToHere((void *)&int148handler);
 idt.myEntry[149].jumpToHere((void *)&int149handler);

 idt.myEntry[150].jumpToHere((void *)&int150handler);
 idt.myEntry[151].jumpToHere((void *)&int151handler);
 idt.myEntry[152].jumpToHere((void *)&int152handler);
 idt.myEntry[153].jumpToHere((void *)&int153handler);
 idt.myEntry[154].jumpToHere((void *)&int154handler);
 idt.myEntry[155].jumpToHere((void *)&int155handler);
 idt.myEntry[156].jumpToHere((void *)&int156handler);
 idt.myEntry[157].jumpToHere((void *)&int157handler);
 idt.myEntry[158].jumpToHere((void *)&int158handler);
 idt.myEntry[159].jumpToHere((void *)&int159handler);

 idt.myEntry[160].jumpToHere((void *)&int160handler);
 idt.myEntry[161].jumpToHere((void *)&int161handler);
 idt.myEntry[162].jumpToHere((void *)&int162handler);
 idt.myEntry[163].jumpToHere((void *)&int163handler);
 idt.myEntry[164].jumpToHere((void *)&int164handler);
 idt.myEntry[165].jumpToHere((void *)&int165handler);
 idt.myEntry[166].jumpToHere((void *)&int166handler);
 idt.myEntry[167].jumpToHere((void *)&int167handler);
 idt.myEntry[168].jumpToHere((void *)&int168handler);
 idt.myEntry[169].jumpToHere((void *)&int169handler);

 idt.myEntry[170].jumpToHere((void *)&int170handler);
 idt.myEntry[171].jumpToHere((void *)&int171handler);
 idt.myEntry[172].jumpToHere((void *)&int172handler);
 idt.myEntry[173].jumpToHere((void *)&int173handler);
 idt.myEntry[174].jumpToHere((void *)&int174handler);
 idt.myEntry[175].jumpToHere((void *)&int175handler);
 idt.myEntry[176].jumpToHere((void *)&int176handler);
 idt.myEntry[177].jumpToHere((void *)&int177handler);
 idt.myEntry[178].jumpToHere((void *)&int178handler);
 idt.myEntry[179].jumpToHere((void *)&int179handler);

 idt.myEntry[180].jumpToHere((void *)&int180handler);
 idt.myEntry[181].jumpToHere((void *)&int181handler);
 idt.myEntry[182].jumpToHere((void *)&int182handler);
 idt.myEntry[183].jumpToHere((void *)&int183handler);
 idt.myEntry[184].jumpToHere((void *)&int184handler);
 idt.myEntry[185].jumpToHere((void *)&int185handler);
 idt.myEntry[186].jumpToHere((void *)&int186handler);
 idt.myEntry[187].jumpToHere((void *)&int187handler);
 idt.myEntry[188].jumpToHere((void *)&int188handler);
 idt.myEntry[189].jumpToHere((void *)&int189handler);

 idt.myEntry[190].jumpToHere((void *)&int190handler);
 idt.myEntry[191].jumpToHere((void *)&int191handler);
 idt.myEntry[192].jumpToHere((void *)&int192handler);
 idt.myEntry[193].jumpToHere((void *)&int193handler);
 idt.myEntry[194].jumpToHere((void *)&int194handler);
 idt.myEntry[195].jumpToHere((void *)&int195handler);
 idt.myEntry[196].jumpToHere((void *)&int196handler);
 idt.myEntry[197].jumpToHere((void *)&int197handler);
 idt.myEntry[198].jumpToHere((void *)&int198handler);
 idt.myEntry[199].jumpToHere((void *)&int199handler);

 idt.myEntry[200].jumpToHere((void *)&int200handler);
 idt.myEntry[201].jumpToHere((void *)&int201handler);
 idt.myEntry[202].jumpToHere((void *)&int202handler);
 idt.myEntry[203].jumpToHere((void *)&int203handler);
 idt.myEntry[204].jumpToHere((void *)&int204handler);
 idt.myEntry[205].jumpToHere((void *)&int205handler);
 idt.myEntry[206].jumpToHere((void *)&int206handler);
 idt.myEntry[207].jumpToHere((void *)&int207handler);
 idt.myEntry[208].jumpToHere((void *)&int208handler);
 idt.myEntry[209].jumpToHere((void *)&int209handler);

 idt.myEntry[210].jumpToHere((void *)&int210handler);
 idt.myEntry[211].jumpToHere((void *)&int211handler);
 idt.myEntry[212].jumpToHere((void *)&int212handler);
 idt.myEntry[213].jumpToHere((void *)&int213handler);
 idt.myEntry[214].jumpToHere((void *)&int214handler);
 idt.myEntry[215].jumpToHere((void *)&int215handler);
 idt.myEntry[216].jumpToHere((void *)&int216handler);
 idt.myEntry[217].jumpToHere((void *)&int217handler);
 idt.myEntry[218].jumpToHere((void *)&int218handler);
 idt.myEntry[219].jumpToHere((void *)&int219handler);

 idt.myEntry[220].jumpToHere((void *)&int220handler);
 idt.myEntry[221].jumpToHere((void *)&int221handler);
 idt.myEntry[222].jumpToHere((void *)&int222handler);
 idt.myEntry[223].jumpToHere((void *)&int223handler);
 idt.myEntry[224].jumpToHere((void *)&int224handler);
 idt.myEntry[225].jumpToHere((void *)&int225handler);
 idt.myEntry[226].jumpToHere((void *)&int226handler);
 idt.myEntry[227].jumpToHere((void *)&int227handler);
 idt.myEntry[228].jumpToHere((void *)&int228handler);
 idt.myEntry[229].jumpToHere((void *)&int229handler);

 idt.myEntry[230].jumpToHere((void *)&int230handler);
 idt.myEntry[231].jumpToHere((void *)&int231handler);
 idt.myEntry[232].jumpToHere((void *)&int232handler);
 idt.myEntry[233].jumpToHere((void *)&int233handler);
 idt.myEntry[234].jumpToHere((void *)&int234handler);
 idt.myEntry[235].jumpToHere((void *)&int235handler);
 idt.myEntry[236].jumpToHere((void *)&int236handler);
 idt.myEntry[237].jumpToHere((void *)&int237handler);
 idt.myEntry[238].jumpToHere((void *)&int238handler);
 idt.myEntry[239].jumpToHere((void *)&int239handler);

 idt.myEntry[240].jumpToHere((void *)&int240handler);
 idt.myEntry[241].jumpToHere((void *)&int241handler);
 idt.myEntry[242].jumpToHere((void *)&int242handler);
 idt.myEntry[243].jumpToHere((void *)&int243handler);
 idt.myEntry[244].jumpToHere((void *)&int244handler);
 idt.myEntry[245].jumpToHere((void *)&int245handler);
 idt.myEntry[246].jumpToHere((void *)&int246handler);
 idt.myEntry[247].jumpToHere((void *)&int247handler);
 idt.myEntry[248].jumpToHere((void *)&int248handler);
 idt.myEntry[249].jumpToHere((void *)&int249handler);

 idt.myEntry[250].jumpToHere((void *)&int250handler);
 idt.myEntry[251].jumpToHere((void *)&int251handler);
 idt.myEntry[252].jumpToHere((void *)&int252handler);
 idt.myEntry[253].jumpToHere((void *)&int253handler);
 idt.myEntry[254].jumpToHere((void *)&int254handler);
 idt.myEntry[255].jumpToHere((void *)&int255handler);
}

