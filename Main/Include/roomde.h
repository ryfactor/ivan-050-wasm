#ifndef __ROOMDE_H__
#define __ROOMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "roomba.h"

class ROOM
(
  normalroom,
  room,
  ;
);

class ROOM
(
  shop,
  room,
 public:
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*, ushort);
  virtual bool DropItem(character*, item*, ushort);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*, ushort);
  virtual bool AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual bool AllowSpoil(const item*) const;
  virtual bool AllowKick(const character*) const;
);

class ROOM
(
  temple,
  room,
 public:
  virtual void Enter(character*);
);

class ROOM
(
  cathedral,
  room,
 public:
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*, ushort);
  virtual bool DropItem(character*, item*, ushort);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*, ushort);
  virtual void SetEntered(bool What) { Entered = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool AllowDropGifts() const { return false; }
  virtual bool Drink(character*) const;
  virtual bool HasDrinkHandler() const { return true; }
  virtual bool Dip(character*) const;
  virtual bool HasDipHandler() const { return true; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual bool AllowSpoil(const item*) const { return false; }
  virtual short GetGodRelationAdjustment() const { return -150; }
 protected:
  virtual void VirtualConstructor(bool);
  bool Entered;
);

class ROOM
(
  library,
  room,
 public:
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*, ushort);
  virtual bool DropItem(character*, item*, ushort);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*, ushort);
  virtual bool AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
);

class ROOM
(
  landingsite,
  room,
 public:
  virtual bool PickupItem(character*, item*, ushort);
  virtual bool DropItem(character*, item*, ushort);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*, ushort);
  virtual bool AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
);

#endif
