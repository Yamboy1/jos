#ifndef JJ386INT_H
#define JJ386INT_H

/*
 * The contents of this file are subject to the JJOS Public License
 * Version 0.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License
 * at http://www.jos.org/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is JJOS code, released October, 1998.
 *
 * The Initial Developer of the Original Code is John Morrison.
 * Portions created by John Morrison are Copyright (C) 1998.
 * All rights reserved.
 *
 * Contributor(s):
 *
 */

#include "jbmachin.h"
#include "jbfifo.h"
#include "jjtypes.h"

class jj386Interrupts: public jbMachine
{
 public:
  jj386Interrupts();

  void		assignInterrupts(void);
  void		enableInterrupts(void);
  void		disableInterrupts(void);

  jjBoolean	interruptFIFOEmpty(void);
  jju16		popOldestInterruptNumber(void);

 protected:

  void buildIDT(void);

  /* Where the interrupt recordings are actually stashed. */

  jbFIFO<jju16, 4> myRecordedInterrupts;

  /* Interrupt handlers (called from asm in jjidt.s86. */

  static void interrupt000Handler(void);
  static void interrupt001Handler(void);
  static void interrupt002Handler(void);
  static void interrupt003Handler(void);
  static void interrupt004Handler(void);
  static void interrupt005Handler(void);
  static void interrupt006Handler(void);
  static void interrupt007Handler(void);
  static void interrupt008Handler(void);
  static void interrupt009Handler(void);

  static void interrupt010Handler(void);
  static void interrupt011Handler(void);
  static void interrupt012Handler(void);
  static void interrupt013Handler(void);
  static void interrupt014Handler(void);
  static void interrupt015Handler(void);
  static void interrupt016Handler(void);
  static void interrupt017Handler(void);
  static void interrupt018Handler(void);
  static void interrupt019Handler(void);

  static void interrupt020Handler(void);
  static void interrupt021Handler(void);
  static void interrupt022Handler(void);
  static void interrupt023Handler(void);
  static void interrupt024Handler(void);
  static void interrupt025Handler(void);
  static void interrupt026Handler(void);
  static void interrupt027Handler(void);
  static void interrupt028Handler(void);
  static void interrupt029Handler(void);

  static void interrupt030Handler(void);
  static void interrupt031Handler(void);
  static void interrupt032Handler(void);
  static void interrupt033Handler(void);
  static void interrupt034Handler(void);
  static void interrupt035Handler(void);
  static void interrupt036Handler(void);
  static void interrupt037Handler(void);
  static void interrupt038Handler(void);
  static void interrupt039Handler(void);

  static void interrupt040Handler(void);
  static void interrupt041Handler(void);
  static void interrupt042Handler(void);
  static void interrupt043Handler(void);
  static void interrupt044Handler(void);
  static void interrupt045Handler(void);
  static void interrupt046Handler(void);
  static void interrupt047Handler(void);
  static void interrupt048Handler(void);
  static void interrupt049Handler(void);

  static void interrupt050Handler(void);
  static void interrupt051Handler(void);
  static void interrupt052Handler(void);
  static void interrupt053Handler(void);
  static void interrupt054Handler(void);
  static void interrupt055Handler(void);
  static void interrupt056Handler(void);
  static void interrupt057Handler(void);
  static void interrupt058Handler(void);
  static void interrupt059Handler(void);

  static void interrupt060Handler(void);
  static void interrupt061Handler(void);
  static void interrupt062Handler(void);
  static void interrupt063Handler(void);
  static void interrupt064Handler(void);
  static void interrupt065Handler(void);
  static void interrupt066Handler(void);
  static void interrupt067Handler(void);
  static void interrupt068Handler(void);
  static void interrupt069Handler(void);

  static void interrupt070Handler(void);
  static void interrupt071Handler(void);
  static void interrupt072Handler(void);
  static void interrupt073Handler(void);
  static void interrupt074Handler(void);
  static void interrupt075Handler(void);
  static void interrupt076Handler(void);
  static void interrupt077Handler(void);
  static void interrupt078Handler(void);
  static void interrupt079Handler(void);

  static void interrupt080Handler(void);
  static void interrupt081Handler(void);
  static void interrupt082Handler(void);
  static void interrupt083Handler(void);
  static void interrupt084Handler(void);
  static void interrupt085Handler(void);
  static void interrupt086Handler(void);
  static void interrupt087Handler(void);
  static void interrupt088Handler(void);
  static void interrupt089Handler(void);

  static void interrupt090Handler(void);
  static void interrupt091Handler(void);
  static void interrupt092Handler(void);
  static void interrupt093Handler(void);
  static void interrupt094Handler(void);
  static void interrupt095Handler(void);
  static void interrupt096Handler(void);
  static void interrupt097Handler(void);
  static void interrupt098Handler(void);
  static void interrupt099Handler(void);

  static void interrupt100Handler(void);
  static void interrupt101Handler(void);
  static void interrupt102Handler(void);
  static void interrupt103Handler(void);
  static void interrupt104Handler(void);
  static void interrupt105Handler(void);
  static void interrupt106Handler(void);
  static void interrupt107Handler(void);
  static void interrupt108Handler(void);
  static void interrupt109Handler(void);

  static void interrupt110Handler(void);
  static void interrupt111Handler(void);
  static void interrupt112Handler(void);
  static void interrupt113Handler(void);
  static void interrupt114Handler(void);
  static void interrupt115Handler(void);
  static void interrupt116Handler(void);
  static void interrupt117Handler(void);
  static void interrupt118Handler(void);
  static void interrupt119Handler(void);

  static void interrupt120Handler(void);
  static void interrupt121Handler(void);
  static void interrupt122Handler(void);
  static void interrupt123Handler(void);
  static void interrupt124Handler(void);
  static void interrupt125Handler(void);
  static void interrupt126Handler(void);
  static void interrupt127Handler(void);
  static void interrupt128Handler(void);
  static void interrupt129Handler(void);

  static void interrupt130Handler(void);
  static void interrupt131Handler(void);
  static void interrupt132Handler(void);
  static void interrupt133Handler(void);
  static void interrupt134Handler(void);
  static void interrupt135Handler(void);
  static void interrupt136Handler(void);
  static void interrupt137Handler(void);
  static void interrupt138Handler(void);
  static void interrupt139Handler(void);

  static void interrupt140Handler(void);
  static void interrupt141Handler(void);
  static void interrupt142Handler(void);
  static void interrupt143Handler(void);
  static void interrupt144Handler(void);
  static void interrupt145Handler(void);
  static void interrupt146Handler(void);
  static void interrupt147Handler(void);
  static void interrupt148Handler(void);
  static void interrupt149Handler(void);

  static void interrupt150Handler(void);
  static void interrupt151Handler(void);
  static void interrupt152Handler(void);
  static void interrupt153Handler(void);
  static void interrupt154Handler(void);
  static void interrupt155Handler(void);
  static void interrupt156Handler(void);
  static void interrupt157Handler(void);
  static void interrupt158Handler(void);
  static void interrupt159Handler(void);

  static void interrupt160Handler(void);
  static void interrupt161Handler(void);
  static void interrupt162Handler(void);
  static void interrupt163Handler(void);
  static void interrupt164Handler(void);
  static void interrupt165Handler(void);
  static void interrupt166Handler(void);
  static void interrupt167Handler(void);
  static void interrupt168Handler(void);
  static void interrupt169Handler(void);

  static void interrupt170Handler(void);
  static void interrupt171Handler(void);
  static void interrupt172Handler(void);
  static void interrupt173Handler(void);
  static void interrupt174Handler(void);
  static void interrupt175Handler(void);
  static void interrupt176Handler(void);
  static void interrupt177Handler(void);
  static void interrupt178Handler(void);
  static void interrupt179Handler(void);

  static void interrupt180Handler(void);
  static void interrupt181Handler(void);
  static void interrupt182Handler(void);
  static void interrupt183Handler(void);
  static void interrupt184Handler(void);
  static void interrupt185Handler(void);
  static void interrupt186Handler(void);
  static void interrupt187Handler(void);
  static void interrupt188Handler(void);
  static void interrupt189Handler(void);

  static void interrupt190Handler(void);
  static void interrupt191Handler(void);
  static void interrupt192Handler(void);
  static void interrupt193Handler(void);
  static void interrupt194Handler(void);
  static void interrupt195Handler(void);
  static void interrupt196Handler(void);
  static void interrupt197Handler(void);
  static void interrupt198Handler(void);
  static void interrupt199Handler(void);

  static void interrupt200Handler(void);
  static void interrupt201Handler(void);
  static void interrupt202Handler(void);
  static void interrupt203Handler(void);
  static void interrupt204Handler(void);
  static void interrupt205Handler(void);
  static void interrupt206Handler(void);
  static void interrupt207Handler(void);
  static void interrupt208Handler(void);
  static void interrupt209Handler(void);

  static void interrupt210Handler(void);
  static void interrupt211Handler(void);
  static void interrupt212Handler(void);
  static void interrupt213Handler(void);
  static void interrupt214Handler(void);
  static void interrupt215Handler(void);
  static void interrupt216Handler(void);
  static void interrupt217Handler(void);
  static void interrupt218Handler(void);
  static void interrupt219Handler(void);

  static void interrupt220Handler(void);
  static void interrupt221Handler(void);
  static void interrupt222Handler(void);
  static void interrupt223Handler(void);
  static void interrupt224Handler(void);
  static void interrupt225Handler(void);
  static void interrupt226Handler(void);
  static void interrupt227Handler(void);
  static void interrupt228Handler(void);
  static void interrupt229Handler(void);

  static void interrupt230Handler(void);
  static void interrupt231Handler(void);
  static void interrupt232Handler(void);
  static void interrupt233Handler(void);
  static void interrupt234Handler(void);
  static void interrupt235Handler(void);
  static void interrupt236Handler(void);
  static void interrupt237Handler(void);
  static void interrupt238Handler(void);
  static void interrupt239Handler(void);

  static void interrupt240Handler(void);
  static void interrupt241Handler(void);
  static void interrupt242Handler(void);
  static void interrupt243Handler(void);
  static void interrupt244Handler(void);
  static void interrupt245Handler(void);
  static void interrupt246Handler(void);
  static void interrupt247Handler(void);
  static void interrupt248Handler(void);
  static void interrupt249Handler(void);

  static void interrupt250Handler(void);
  static void interrupt251Handler(void);
  static void interrupt252Handler(void);
  static void interrupt253Handler(void);
  static void interrupt254Handler(void);
  static void interrupt255Handler(void);
};

#endif
