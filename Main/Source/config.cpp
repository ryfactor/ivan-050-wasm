#include <fstream>

#include "config.h"
#include "felist.h"
#include "graphics.h"
#include "strover.h"
#include "feio.h"
#include "game.h"
#include "area.h"

#ifdef WIN32
#define CONFIG_FILENAME "ivan.cfg"
#endif

#ifdef __DJGPP__
#define CONFIG_FILENAME "ivan.cfg"
#endif

#ifdef USE_SDL
#define CONFIG_FILENAME (std::string(getenv("HOME")) + std::string("/.ivan.conf")).c_str()
#endif

std::string configuration::DefaultName;
ushort configuration::AutoSaveInterval = 500;
uchar configuration::Contrast = 100;
bool configuration::AutoDropLeftOvers = true;
bool configuration::OutlineCharacters = false;
bool configuration::OutlineItems = false;
ushort configuration::CharacterOutlineColor = BLUE;
ushort configuration::ItemOutlineColor = RED;
bool configuration::BeepOnCritical = false;
bool configuration::FullScreenMode = false;

void configuration::Save()
{
  std::ofstream SaveFile(CONFIG_FILENAME, std::ios::out);

  if(!SaveFile.is_open())
    return;

  SaveFile << "DefaultName = \"" << DefaultName << "\";\n";
  SaveFile << "AutoSaveInterval = " << AutoSaveInterval << ";\n";
  SaveFile << "Contrast = " << ulong(Contrast) << ";\n";
  SaveFile << "AutoDropLeftOvers = " << AutoDropLeftOvers << ";\n";
  SaveFile << "OutlineCharacters = " << OutlineCharacters << ";\n";
  SaveFile << "OutlineItems = " << OutlineItems << ";\n";
  SaveFile << "CharacterOutlineColor = " << GET_RED(CharacterOutlineColor) << ", " << GET_GREEN(CharacterOutlineColor) << ", " << GET_BLUE(CharacterOutlineColor) << ";\n";
  SaveFile << "ItemOutlineColor = " << GET_RED(ItemOutlineColor) << ", " << GET_GREEN(ItemOutlineColor) << ", " << GET_BLUE(ItemOutlineColor) << ";\n";
  SaveFile << "BeepOnCritical = " << BeepOnCritical << ";\n";
  SaveFile << "FullScreenMode = " << FullScreenMode << ";\n";
}

void configuration::Load()
{
  inputfile SaveFile(CONFIG_FILENAME, false);

  if(!SaveFile.IsOpen())
    return;

  for(std::string Word = SaveFile.ReadWord(false); !SaveFile.Eof(); Word = SaveFile.ReadWord(false))
    {
      if(Word == "DefaultName")
	{
	  SaveFile.ReadWord();
	  SetDefaultName(SaveFile.ReadWord());
	}

      if(Word == "AutoSaveInterval")
	SetAutoSaveInterval(SaveFile.ReadNumber());

      if(Word == "Contrast")
	SetContrast(SaveFile.ReadNumber());

      if(Word == "AutoDropLeftOvers")
	SetAutoDropLeftOvers(SaveFile.ReadBool());

      if(Word == "OutlineCharacters")
	SetOutlineCharacters(SaveFile.ReadBool());

      if(Word == "OutlineItems")
	SetOutlineItems(SaveFile.ReadBool());

      if(Word == "CharacterOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetCharacterOutlineColor(MAKE_RGB(Red, Green, Blue));
	}

      if(Word == "ItemOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetItemOutlineColor(MAKE_RGB(Red, Green, Blue));
	}

      if(Word == "BeepOnCritical")
	SetBeepOnCritical(SaveFile.ReadBool());

      if(Word == "FullScreenMode")
	SetFullScreenMode(SaveFile.ReadBool());
    }
}

void configuration::EditContrast(char Change)
{
  short TContrast = Contrast;

  TContrast += Change;

  if(TContrast < 0) TContrast = 0;
  if(TContrast > 200) TContrast = 200;

  Contrast = TContrast;
}

#include "charde.h"
#include "itemde.h"

void configuration::ShowConfigScreen()
{
  vector2d QuestionPos = game::GetRunning() ? vector2d(16, 6) : vector2d(30, 46);
  ushort Chosen = 0;
  bool BoolChange = false;

  while(true)
    {
      if(game::GetRunning())
	game::DrawEverythingNoBlit();

      felist List("Which setting do you wish to configure?", WHITE);

      List.SetSelected(Chosen);

      List.AddDescription("");
      List.AddDescription("Setting                                    Value");

      List.AddEntry(std::string("Player's default name:                  ") + (DefaultName == "" ? "-" : DefaultName), LIGHTGRAY);
      List.AddEntry(std::string("Autosave interval:                      ") + AutoSaveInterval + " turns", LIGHTGRAY);
      List.AddEntry(std::string("Contrast:                               ") + Contrast + "/100", LIGHTGRAY);
      List.AddEntry(std::string("Drop food leftovers automatically:      ") + (AutoDropLeftOvers ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Outline all characters:                 ") + (OutlineCharacters ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Outline all items:                      ") + (OutlineItems ? "yes" : "no"), LIGHTGRAY);

#ifdef WIN32
      List.AddEntry(std::string("Beep on critical messages:              ") + (BeepOnCritical ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Run the game in full screen mode:       ") + (FullScreenMode ? "yes" : "no"), LIGHTGRAY);
#endif

      Chosen = List.Draw(vector2d(10, game::GetRunning() ? 42 : 10), 780, 20, true, false, game::GetRunning(), !game::GetRunning() && !BoolChange);

      switch(Chosen)
	{
	case 0:
	  SetDefaultName(iosystem::StringQuestion("Set new default name (3-20 letters):", QuestionPos, WHITE, 0, 20, !game::GetRunning(), true));
	  BoolChange = false;
	  continue;
	case 1:
	  SetAutoSaveInterval(iosystem::NumberQuestion("Set new autosave interval (1-50000 turns, 0 for never):", QuestionPos, WHITE, !game::GetRunning()));
	  BoolChange = false;
	  continue;
	case 2:
	  SetContrast(iosystem::NumberQuestion("Set new contrast value (0-200):", QuestionPos, WHITE, !game::GetRunning()));
	  if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = false;
	  continue;
	case 3:
	  SetAutoDropLeftOvers(!GetAutoDropLeftOvers());
	  BoolChange = true;
	  continue;
	case 4:
	  SetOutlineCharacters(!GetOutlineCharacters());
	  if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  continue;
	case 5:
	  SetOutlineItems(!GetOutlineItems());
	  if(game::GetRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  continue;
#ifdef WIN32
	case 6:
	  SetBeepOnCritical(!GetBeepOnCritical());
	  BoolChange = true;
	  continue;
	case 7:
	  graphics::SwitchMode();
	  BoolChange = true;
	  continue;
#endif
	}

      break;
    }
}

void configuration::SetDefaultName(std::string What)
{
  DefaultName = What;

  if(DefaultName.length() < 3)
    DefaultName = "";
}

void configuration::SetAutoSaveInterval(long What)
{
  if(What < 0) What = 0;
  if(What > 50000) What = 50000;

  AutoSaveInterval = What;
}

void configuration::SetContrast(long What)
{
  if(What < 0) What = 0;
  if(What > 200) What = 200;

  Contrast = What;
}

void configuration::SwitchModeHandler()
{
  SetFullScreenMode(!GetFullScreenMode());
}

