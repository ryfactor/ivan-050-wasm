/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through dataset.cpp */

databasecreator<character>::databasemembermap databasecreator<character>::DataBaseMemberMap;
databasecreator<item>::databasemembermap databasecreator<item>::DataBaseMemberMap;
databasecreator<glterrain>::databasemembermap databasecreator<glterrain>::DataBaseMemberMap;
databasecreator<olterrain>::databasemembermap databasecreator<olterrain>::DataBaseMemberMap;
databasecreator<material>::databasemembermap databasecreator<material>::DataBaseMemberMap;

int CreateConfigTable(databasebase*** ConfigTable, databasebase*** TempTable, databasebase** TempConfig, long* TempTableInfo, int Type, int Configs, int TempTables)
{
  memset(ConfigTable, 0, CONFIG_TABLE_SIZE * sizeof(databasebase**));

  for(int c = 0; c < Configs; ++c)
    {
      int Config = TempConfig[c]->Config;
      int Hash = Config >> 8 ^ Config & 0xFF;

      if((TempTableInfo[Hash] & 0xFFFF) != Type)
	{
	  TempTable[0][Hash] = TempConfig[c];
	  TempTableInfo[Hash] = Type | 0x10000;
	}
      else
	{
	  int Conflicts = (TempTableInfo[Hash] & 0xFFFF0000) >> 16;

	  if(Conflicts == TempTables)
	    TempTable[TempTables++] = new databasebase*[CONFIG_TABLE_SIZE];

	  TempTable[Conflicts][Hash] = TempConfig[c];
	  TempTableInfo[Hash] += 0x10000;
	}
    }

  for(int c1 = 0; c1 < CONFIG_TABLE_SIZE; ++c1)
    if((TempTableInfo[c1] & 0xFFFF) == Type)
      {
	int Entries = (TempTableInfo[c1] & 0xFFFF0000) >> 16;
	ConfigTable[c1] = new databasebase*[Entries + 1];

	for(int c2 = 0; c2 < Entries; ++c2)
	  ConfigTable[c1][c2] = TempTable[c2][c1];

	ConfigTable[c1][Entries] = 0;
      }

  return TempTables;
}

template <class type> void databasecreator<type>::ReadFrom(inputfile& SaveFile)
{
  typedef typename type::prototype prototype;
  festring Word;
  database* TempConfig[1024];
  databasebase** TempTable[1024];
  long TempTableInfo[CONFIG_TABLE_SIZE];
  int TempTables = 1;
  databasebase* FirstTempTable[CONFIG_TABLE_SIZE];
  TempTable[0] = FirstTempTable;
  memset(TempTableInfo, 0, CONFIG_TABLE_SIZE * sizeof(long));
  CreateDataBaseMemberMap();

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      int Type = protocontainer<type>::SearchCodeName(Word);

      if(!Type)
	ABORT("Odd term %s encountered in %s datafile line %d!", Word.CStr(), protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

      prototype* Proto = protocontainer<type>::ProtoData[Type];
      database* DataBase = Proto->Base ? new database(**Proto->Base->ConfigData) : new database;
      DataBase->InitDefaults(Proto, 0);
      TempConfig[0] = DataBase;
      int Configs = 1;

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  if(Word == "Config")
	    {
	      int ConfigNumber = SaveFile.ReadNumber();
	      database* ConfigDataBase = new database(*Proto->ChooseBaseForConfig(TempConfig, Configs, ConfigNumber));//&Proto->Config.insert(std::pair<int, database>(ConfigNumber, Proto->ChooseBaseForConfig(ConfigNumber))).first->second;
	      ConfigDataBase->InitDefaults(Proto, ConfigNumber);
	      TempConfig[Configs++] = ConfigDataBase;

	      if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

	      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
		if(!AnalyzeData(SaveFile, Word, *ConfigDataBase))
		  ABORT("Illegal datavalue %s found while building up %s config #%d, line %d!", Word.CStr(), Proto->GetClassID(), ConfigNumber, SaveFile.TellLine());

	      ConfigDataBase->PostProcess();
	      continue;
	    }

	  if(!AnalyzeData(SaveFile, Word, *DataBase))
	    ABORT("Illegal datavalue %s found while building up %s, line %d!", Word.CStr(), Proto->GetClassID(), SaveFile.TellLine());
	}

      DataBase->PostProcess();
      Configs = Proto->CreateSpecialConfigurations(TempConfig, Configs);
      Proto->ConfigData = new database*[Configs];
      Proto->ConfigSize = Configs;
      memcpy(Proto->ConfigData, TempConfig, Configs * sizeof(database*));
      TempTables =
      CreateConfigTable(reinterpret_cast<databasebase***>(Proto->ConfigTable),
			TempTable,
			reinterpret_cast<databasebase**>(TempConfig),
			TempTableInfo, Type, Configs, TempTables);
    }

  for(int c = 1; c < TempTables; ++c)
    delete [] TempTable[c];

  DataBaseMemberMap.clear();
}

template <class type> int databasecreator<type>::CreateDivineConfigurations(const prototype* Proto, database** TempConfig, int Configs)
{
  int OldConfigs = Configs;

  for(int c1 = 1; c1 < protocontainer<god>::GetSize(); ++c1)
    {
      database* ConfigDataBase = 0;

      for(int c2 = 1; c2 < OldConfigs; ++c2)
	{
	  ConfigDataBase = TempConfig[c2];

	  if(ConfigDataBase->Config == c1)
	    break;
	}

      bool Created = false;

      if(!ConfigDataBase)
	{
	  ConfigDataBase = new database(**TempConfig);
	  ConfigDataBase->InitDefaults(Proto, c1);
	  Created = true;
	}

      ConfigDataBase->AttachedGod = c1;
      ConfigDataBase->PostFix << "of " << festring(protocontainer<god>::GetProto(c1)->GetClassID()).CapitalizeCopy();

      if(Created)
	TempConfig[Configs++] = ConfigDataBase;
    }

  return Configs;
}

template int databasecreator<character>::CreateDivineConfigurations(const prototype*, database**, int);
template int databasecreator<item>::CreateDivineConfigurations(const prototype*, database**, int);
template int databasecreator<olterrain>::CreateDivineConfigurations(const prototype*, database**, int);

template <class database, class member> struct databasemember : public databasememberbase<database>
{
  databasemember(member Member) : Member(Member) { }
  virtual void ReadData(database& DataBase, inputfile& SaveFile) { ::ReadData(DataBase.*Member, SaveFile); }
  member Member;
};

template <class database, class member> void AddMember(std::map<festring, databasememberbase<database>*>& Map, const char* Str, member Member)
{
  Map.insert(std::pair<festring, databasememberbase<database>*>(Str, new databasemember<database, member>(Member)));
}

/* Explicit instantiations seem to increase compile speed greatly here... */

#define INST_ADD_MEMBER(type, member)\
template void AddMember<type##database, member type##database::*>(std::map<festring, databasememberbase<type##database>*>&, const char*, member type##database::*)

INST_ADD_MEMBER(character, bool);
INST_ADD_MEMBER(character, int);
INST_ADD_MEMBER(character, long);
INST_ADD_MEMBER(character, packedcolor16);
INST_ADD_MEMBER(character, color24);
INST_ADD_MEMBER(character, packedvector2d);
INST_ADD_MEMBER(character, festring);
INST_ADD_MEMBER(character, fearray<long>);
INST_ADD_MEMBER(character, fearray<festring>);
INST_ADD_MEMBER(character, contentscript<item>);

INST_ADD_MEMBER(item, bool);
INST_ADD_MEMBER(item, int);
INST_ADD_MEMBER(item, long);
INST_ADD_MEMBER(item, color24);
INST_ADD_MEMBER(item, vector2d);
INST_ADD_MEMBER(item, festring);
INST_ADD_MEMBER(item, fearray<long>);
INST_ADD_MEMBER(item, fearray<festring>);

INST_ADD_MEMBER(glterrain, bool);
INST_ADD_MEMBER(glterrain, int);
INST_ADD_MEMBER(glterrain, long);
INST_ADD_MEMBER(glterrain, vector2d);
INST_ADD_MEMBER(glterrain, festring);
INST_ADD_MEMBER(glterrain, fearray<long>);

INST_ADD_MEMBER(olterrain, bool);
INST_ADD_MEMBER(olterrain, int);
INST_ADD_MEMBER(olterrain, long);
INST_ADD_MEMBER(olterrain, vector2d);
INST_ADD_MEMBER(olterrain, festring);
INST_ADD_MEMBER(olterrain, fearray<long>);
INST_ADD_MEMBER(olterrain, fearray<contentscript<item> >);

INST_ADD_MEMBER(material, bool);
INST_ADD_MEMBER(material, int);
INST_ADD_MEMBER(material, long);
INST_ADD_MEMBER(material, color24);
INST_ADD_MEMBER(material, festring);
INST_ADD_MEMBER(material, contentscript<item>);

#define ADD_MEMBER(data) AddMember(DataBaseMemberMap, #data, &database::data);

template<> void databasecreator<character>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(DefaultArmStrength);
  ADD_MEMBER(DefaultLegStrength);
  ADD_MEMBER(DefaultDexterity);
  ADD_MEMBER(DefaultAgility);
  ADD_MEMBER(DefaultEndurance);
  ADD_MEMBER(DefaultPerception);
  ADD_MEMBER(DefaultIntelligence);
  ADD_MEMBER(DefaultWisdom);
  ADD_MEMBER(DefaultCharisma);
  ADD_MEMBER(DefaultMana);
  ADD_MEMBER(DefaultMoney);
  ADD_MEMBER(TotalSize);
  ADD_MEMBER(CanRead);
  ADD_MEMBER(IsCharmable);
  ADD_MEMBER(Sex);
  ADD_MEMBER(CanBeGenerated);
  ADD_MEMBER(CriticalModifier);
  ADD_MEMBER(StandVerb);
  ADD_MEMBER(CanOpen);
  ADD_MEMBER(CanBeDisplaced);
  ADD_MEMBER(Frequency);
  ADD_MEMBER(EnergyResistance);
  ADD_MEMBER(FireResistance);
  ADD_MEMBER(PoisonResistance);
  ADD_MEMBER(ElectricityResistance);
  ADD_MEMBER(AcidResistance);
  ADD_MEMBER(IsUnique);
  ADD_MEMBER(ConsumeFlags);
  ADD_MEMBER(TotalVolume);
  ADD_MEMBER(HeadBitmapPos);
  ADD_MEMBER(TorsoBitmapPos);
  ADD_MEMBER(ArmBitmapPos);
  ADD_MEMBER(LegBitmapPos);
  ADD_MEMBER(RightArmBitmapPos);
  ADD_MEMBER(LeftArmBitmapPos);
  ADD_MEMBER(RightLegBitmapPos);
  ADD_MEMBER(LeftLegBitmapPos);
  ADD_MEMBER(GroinBitmapPos);
  ADD_MEMBER(ClothColor);
  ADD_MEMBER(SkinColor);
  ADD_MEMBER(CapColor);
  ADD_MEMBER(HairColor);
  ADD_MEMBER(EyeColor);
  ADD_MEMBER(TorsoMainColor);
  ADD_MEMBER(BeltColor);
  ADD_MEMBER(BootColor);
  ADD_MEMBER(TorsoSpecialColor);
  ADD_MEMBER(ArmMainColor);
  ADD_MEMBER(GauntletColor);
  ADD_MEMBER(ArmSpecialColor);
  ADD_MEMBER(LegMainColor);
  ADD_MEMBER(LegSpecialColor);
  ADD_MEMBER(IsNameable);
  ADD_MEMBER(BaseEmitation);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(IsPolymorphable);
  ADD_MEMBER(BaseUnarmedStrength);
  ADD_MEMBER(BaseBiteStrength);
  ADD_MEMBER(BaseKickStrength);
  ADD_MEMBER(AttackStyle);
  ADD_MEMBER(CanUseEquipment);
  ADD_MEMBER(CanKick);
  ADD_MEMBER(CanTalk);
  ADD_MEMBER(ClassStates);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(Alias);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(CreateGolemMaterialConfigurations);
  ADD_MEMBER(Helmet);
  ADD_MEMBER(Amulet);
  ADD_MEMBER(Cloak);
  ADD_MEMBER(BodyArmor);
  ADD_MEMBER(Belt);
  ADD_MEMBER(RightWielded);
  ADD_MEMBER(LeftWielded);
  ADD_MEMBER(RightRing);
  ADD_MEMBER(LeftRing);
  ADD_MEMBER(RightGauntlet);
  ADD_MEMBER(LeftGauntlet);
  ADD_MEMBER(RightBoot);
  ADD_MEMBER(LeftBoot);
  ADD_MEMBER(AttributeBonus);
  ADD_MEMBER(KnownCWeaponSkills);
  ADD_MEMBER(CWeaponSkillHits);
  ADD_MEMBER(RightSWeaponSkillHits);
  ADD_MEMBER(LeftSWeaponSkillHits);
  ADD_MEMBER(PanicLevel);
  ADD_MEMBER(CanBeCloned);
  ADD_MEMBER(Inventory);
  ADD_MEMBER(DangerModifier);
  ADD_MEMBER(DefaultName);
  ADD_MEMBER(FriendlyReplies);
  ADD_MEMBER(HostileReplies);
  ADD_MEMBER(CanZap);
  ADD_MEMBER(FleshMaterial);
  ADD_MEMBER(HasFeet);
  ADD_MEMBER(DeathMessage);
  ADD_MEMBER(IgnoreDanger);
  ADD_MEMBER(HPRequirementForGeneration);
  ADD_MEMBER(DayRequirementForGeneration);
  ADD_MEMBER(IsExtraCoward);
  ADD_MEMBER(SpillsBlood);
  ADD_MEMBER(HasEyes);
  ADD_MEMBER(HasHead);
  ADD_MEMBER(CanThrow);
  ADD_MEMBER(UsesNutrition);
  ADD_MEMBER(AttackWisdomLimit);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BodyPartsDisappearWhenSevered);
  ADD_MEMBER(CanBeConfused);
  ADD_MEMBER(CanApply);
  ADD_MEMBER(WieldedPosition);
  ADD_MEMBER(SparklingFlags);
  ADD_MEMBER(BiteCapturesBodyPart);
  ADD_MEMBER(IsPlant);
  ADD_MEMBER(MoveType);
  ADD_MEMBER(DestroysWalls);
  ADD_MEMBER(IsRooted);
  ADD_MEMBER(BloodMaterial);
  ADD_MEMBER(VomitMaterial);
  ADD_MEMBER(HasSecondaryMaterial);
  ADD_MEMBER(IsImmuneToLeprosy);
  ADD_MEMBER(PolymorphIntelligenceRequirement);
  ADD_MEMBER(AutomaticallySeen);
  ADD_MEMBER(CanHear);
  ADD_MEMBER(DefaultCommandFlags);
  ADD_MEMBER(ConstantCommandFlags);
  ADD_MEMBER(WillCarryItems);
  ADD_MEMBER(ForceVomitMessage);
  ADD_MEMBER(SweatMaterial);
  ADD_MEMBER(Sweats);
}

template<> void databasecreator<item>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(Possibility);
  ADD_MEMBER(IsDestroyable);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(IsMaterialChangeable);
  ADD_MEMBER(WeaponCategory);
  ADD_MEMBER(IsPolymorphSpawnable);
  ADD_MEMBER(IsAutoInitializable);
  ADD_MEMBER(Category);
  ADD_MEMBER(FireResistance);
  ADD_MEMBER(PoisonResistance);
  ADD_MEMBER(ElectricityResistance);
  ADD_MEMBER(AcidResistance);
  ADD_MEMBER(StrengthModifier);
  ADD_MEMBER(FormModifier);
  ADD_MEMBER(DefaultSize);
  ADD_MEMBER(DefaultMainVolume);
  ADD_MEMBER(DefaultSecondaryVolume);
  ADD_MEMBER(BitmapPos);
  ADD_MEMBER(Price);
  ADD_MEMBER(BaseEmitation);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(MainMaterialConfig);
  ADD_MEMBER(SecondaryMaterialConfig);
  ADD_MEMBER(MaterialConfigChances);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(IsPolymorphable);
  ADD_MEMBER(Alias);
  ADD_MEMBER(OKVisualEffects);
  ADD_MEMBER(CanBeGeneratedInContainer);
  ADD_MEMBER(ForcedVisualEffects);
  ADD_MEMBER(Roundness);
  ADD_MEMBER(GearStates);
  ADD_MEMBER(IsTwoHanded);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(CanBeCloned);
  ADD_MEMBER(CanBeMirrored);
  ADD_MEMBER(BeamRange);
  ADD_MEMBER(CanBeBroken);
  ADD_MEMBER(WallBitmapPos);
  ADD_MEMBER(FlexibleNameSingular);
  ADD_MEMBER(MaxCharges);
  ADD_MEMBER(MinCharges);
  ADD_MEMBER(CanBePiled);
  ADD_MEMBER(StorageVolume);
  ADD_MEMBER(MaxGeneratedContainedItems);
  ADD_MEMBER(AffectsArmStrength);
  ADD_MEMBER(AffectsLegStrength);
  ADD_MEMBER(AffectsDexterity);
  ADD_MEMBER(AffectsAgility);
  ADD_MEMBER(AffectsEndurance);
  ADD_MEMBER(AffectsPerception);
  ADD_MEMBER(AffectsIntelligence);
  ADD_MEMBER(AffectsWisdom);
  ADD_MEMBER(AffectsCharisma);
  ADD_MEMBER(AffectsMana);
  ADD_MEMBER(BaseEnchantment);
  ADD_MEMBER(PriceIsProportionalToEnchantment);
  ADD_MEMBER(InElasticityPenaltyModifier);
  ADD_MEMBER(CanBeUsedBySmith);
  ADD_MEMBER(AffectsCarryingCapacity);
  ADD_MEMBER(DamageDivider);
  ADD_MEMBER(HandleInPairs);
  ADD_MEMBER(CanBeEnchanted);
  ADD_MEMBER(BeamColor);
  ADD_MEMBER(BeamEffect);
  ADD_MEMBER(BeamStyle);
  ADD_MEMBER(WearWisdomLimit);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BreakEffectRangeSquare);
  ADD_MEMBER(WieldedBitmapPos);
  ADD_MEMBER(IsQuestItem);
  ADD_MEMBER(IsGoodWithPlants);
  ADD_MEMBER(CreateLockConfigurations);
  ADD_MEMBER(CanBePickedUp);
  ADD_MEMBER(CoverPercentile);
  ADD_MEMBER(TorsoArmorBitmapPos);
  ADD_MEMBER(ArmArmorBitmapPos);
  ADD_MEMBER(LegArmorBitmapPos);
  ADD_MEMBER(HelmetBitmapPos);
  ADD_MEMBER(CloakBitmapPos);
  ADD_MEMBER(BeltBitmapPos);
  ADD_MEMBER(GauntletBitmapPos);
  ADD_MEMBER(BootBitmapPos);
  ADD_MEMBER(HasSecondaryMaterial);
  ADD_MEMBER(AllowEquip);
  ADD_MEMBER(ReadDifficulty);
  ADD_MEMBER(IsValuable);
  ADD_MEMBER(EnchantmentMinusChance);
  ADD_MEMBER(EnchantmentPlusChance);
}

template<class type> void databasecreator<type>::CreateLTerrainDataBaseMemberMap()
{
  ADD_MEMBER(BitmapPos);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(MainMaterialConfig);
  ADD_MEMBER(SecondaryMaterialConfig);
  ADD_MEMBER(MaterialConfigChances);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(OKVisualEffects);
  ADD_MEMBER(MaterialColorB);
  ADD_MEMBER(MaterialColorC);
  ADD_MEMBER(MaterialColorD);
  ADD_MEMBER(SitMessage);
  ADD_MEMBER(ShowMaterial);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(Walkability);
  ADD_MEMBER(HasSecondaryMaterial);
}

template<> void databasecreator<glterrain>::CreateDataBaseMemberMap()
{
  CreateLTerrainDataBaseMemberMap();
}

template<> void databasecreator<olterrain>::CreateDataBaseMemberMap()
{
  CreateLTerrainDataBaseMemberMap();
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(DigMessage);
  ADD_MEMBER(CanBeDestroyed);
  ADD_MEMBER(RestModifier);
  ADD_MEMBER(RestMessage);
  ADD_MEMBER(IsUpLink);
  ADD_MEMBER(StorageVolume);
  ADD_MEMBER(HPModifier);
  ADD_MEMBER(IsSafeToCreateDoor);
  ADD_MEMBER(OpenBitmapPos);
  ADD_MEMBER(CreateLockConfigurations);
  ADD_MEMBER(IsAlwaysTransparent);
  ADD_MEMBER(UseBorderTiles);
  ADD_MEMBER(BorderTilePriority);
  ADD_MEMBER(CreateWindowConfigurations);
  ADD_MEMBER(WindowBitmapPos);
  ADD_MEMBER(ShowThingsUnder);
  ADD_MEMBER(LeftOverItems);
}

template<> void databasecreator<material>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(StrengthValue);
  ADD_MEMBER(ConsumeType);
  ADD_MEMBER(Density);
  ADD_MEMBER(Color);
  ADD_MEMBER(RainColor);
  ADD_MEMBER(PriceModifier);
  ADD_MEMBER(IsGolemMaterial);
  ADD_MEMBER(Emitation);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(NutritionValue);
  ADD_MEMBER(IsAlive);
  ADD_MEMBER(IsFlammable);
  ADD_MEMBER(IsExplosive);
  ADD_MEMBER(NameStem);
  ADD_MEMBER(AdjectiveStem);
  ADD_MEMBER(Article);
  ADD_MEMBER(Effect);
  ADD_MEMBER(ConsumeEndMessage);
  ADD_MEMBER(HitMessage);
  ADD_MEMBER(ExplosivePower);
  ADD_MEMBER(Alpha);
  ADD_MEMBER(Flexibility);
  ADD_MEMBER(SpoilModifier);
  ADD_MEMBER(IsSparkling);
  ADD_MEMBER(EffectStrength);
  ADD_MEMBER(IsMetal);
  ADD_MEMBER(CanHaveParasite);
  ADD_MEMBER(DigProductMaterial);
  ADD_MEMBER(ConsumeWisdomLimit);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BreatheMessage);
  ADD_MEMBER(EffectIsGood);
  ADD_MEMBER(IsWarm);
  ADD_MEMBER(UseMaterialAttributes);
  ADD_MEMBER(CanRegenerate);
  ADD_MEMBER(BreatheWisdomLimit);
  ADD_MEMBER(RustModifier);
  ADD_MEMBER(IsBlood);
  ADD_MEMBER(Acidicity);
  ADD_MEMBER(IsImmuneToAcid);
  ADD_MEMBER(NaturalForm);
  ADD_MEMBER(HardenedMaterial);
  ADD_MEMBER(IntelligenceRequirement);
  ADD_MEMBER(IsScary);
  ADD_MEMBER(CanBeMirrored);
  ADD_MEMBER(AffectInside);
  ADD_MEMBER(IsValuable);
}

template <class type> bool databasecreator<type>::AnalyzeData(inputfile& SaveFile, const festring& Word, database& DataBase)
{
  typename databasemembermap::iterator i = DataBaseMemberMap.find(Word);

  if(i != DataBaseMemberMap.end())
    {
      i->second->ReadData(DataBase, SaveFile);
      CheckDefaults(Word, DataBase);
      return true;
   }
  else
    return false;
}

template<> void databasecreator<character>::CheckDefaults(const festring& Word, character::database& DataBase)
{
  if(Word == "ArmBitmapPos")
    DataBase.RightArmBitmapPos =
    DataBase.LeftArmBitmapPos =
    DataBase.ArmBitmapPos;
  else if(Word == "LegBitmapPos")
    DataBase.GroinBitmapPos =
    DataBase.RightLegBitmapPos =
    DataBase.LeftLegBitmapPos =
    DataBase.LegBitmapPos;
  else if(Word == "ClothColor")
    DataBase.CapColor =
    DataBase.TorsoMainColor =
    DataBase.ArmMainColor =
    DataBase.GauntletColor =
    DataBase.LegMainColor =
    DataBase.ClothColor;
  else if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
  else if(Word == "BaseUnarmedStrength")
    {
      DataBase.BaseBiteStrength = DataBase.BaseUnarmedStrength >> 1;
      DataBase.BaseKickStrength = DataBase.BaseUnarmedStrength << 1;
    }
  else if(Word == "RightGauntlet")
    DataBase.LeftGauntlet = DataBase.RightGauntlet;
  else if(Word == "RightBoot")
    DataBase.LeftBoot = DataBase.RightBoot;
  else if(Word == "DefaultName")
    DataBase.Alias.Add(DataBase.DefaultName);
  else if(Word == "IsUnique")
    DataBase.CanBeWished = !DataBase.IsUnique;
}

template<> void databasecreator<item>::CheckDefaults(const festring& Word, item::database& DataBase)
{
  if(Word == "NameSingular")
    {
      DataBase.NamePlural = DataBase.NameSingular + 's';
      DataBase.FlexibleNameSingular = DataBase.NameSingular;
    }
  else if(Word == "BitmapPos")
    DataBase.WallBitmapPos = DataBase.BitmapPos;
  else if(Word == "MaterialConfigChances")
    DataBase.MaterialConfigChanceSum = femath::SumArray(DataBase.MaterialConfigChances);
  else if(Word == "CanBeCloned")
    DataBase.CanBeMirrored = DataBase.CanBeCloned;
}

template<> void databasecreator<glterrain>::CheckDefaults(const festring& Word, glterrain::database& DataBase)
{
  if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
  else if(Word == "MaterialConfigChances")
    DataBase.MaterialConfigChanceSum = femath::SumArray(DataBase.MaterialConfigChances);
}

template<> void databasecreator<olterrain>::CheckDefaults(const festring& Word, olterrain::database& DataBase)
{
  if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
  else if(Word == "MaterialConfigChances")
    DataBase.MaterialConfigChanceSum = femath::SumArray(DataBase.MaterialConfigChances);
}

template<> void databasecreator<material>::CheckDefaults(const festring& Word, material::database& DataBase)
{
  if(Word == "NameStem")
    DataBase.AdjectiveStem = DataBase.NameStem;
  else if(Word == "Color")
    DataBase.RainColor = DataBase.Color;
}

void databasesystem::Initialize()
{
  {
    /* Must be before character */

    inputfile ScriptFile(game::GetGameDir() + "Script/material.dat", &game::GetGlobalValueMap());
    databasecreator<material>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/char.dat", &game::GetGlobalValueMap());
    databasecreator<character>::ReadFrom(ScriptFile);
  }

  {
    /* Must be before olterrain */

    inputfile ScriptFile(game::GetGameDir() + "Script/item.dat", &game::GetGlobalValueMap());
    databasecreator<item>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/glterra.dat", &game::GetGlobalValueMap());
    databasecreator<glterrain>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/olterra.dat", &game::GetGlobalValueMap());
    databasecreator<olterrain>::ReadFrom(ScriptFile);
  }
}

template <class type> inline void databasecreator<type>::FindDataBase(const database*& DataBase, const prototype* Proto, int Config)
{
  database** Table = Proto->ConfigTable[Config >> 8 ^ Config & 0xFF];

  if(Table)
    {
      if((*Table)->Config == Config)
	{
	  DataBase = *Table;
	  return;
	}
      else
	for(++Table; *Table; ++Table)
	  if((*Table)->Config == Config)
	    {
	      DataBase = *Table;
	      return;
	    }
    }

  DataBase = 0;
}

template void databasecreator<character>::FindDataBase(const database*&, const prototype*, int);
template void databasecreator<material>::FindDataBase(const database*&, const prototype*, int);

template <class type> inline void databasecreator<type>::InstallDataBase(type* Instance, int Config)
{
  const prototype* Proto = Instance->GetProtoType();
  FindDataBase(Instance->DataBase, Proto, Config);

  if(!Instance->DataBase)
    ABORT("Undefined %s configuration #%d sought!", Proto->GetClassID(), Config);
}

#define INST_INSTALL_DATABASE(type)\
template void databasecreator<type>::InstallDataBase(type*, int)

INST_INSTALL_DATABASE(material);
INST_INSTALL_DATABASE(character);
INST_INSTALL_DATABASE(item);
INST_INSTALL_DATABASE(glterrain);
INST_INSTALL_DATABASE(olterrain);
