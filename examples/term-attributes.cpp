/***********************************************************************
* term-attributes.cpp - Test the video attributes of the terminal      *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2015-2020 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <functional>
#include <vector>
#include <final/final.h>

namespace fc = finalcut::fc;
using finalcut::FPoint;
using finalcut::FSize;
using finalcut::FColorPair;


//----------------------------------------------------------------------
// class AttribDlg
//----------------------------------------------------------------------

class AttribDlg final : public finalcut::FDialog
{
  public:
    // Constructor
    explicit AttribDlg (finalcut::FWidget* = nullptr);

    // Disable copy constructor
    AttribDlg (const AttribDlg&) = delete;

    // Destructor
    ~AttribDlg() override = default;

    // Disable copy assignment operator (=)
    AttribDlg& operator = (const AttribDlg&) = delete;

    // Methods
    FColor getBGColor() const;

    // Event handlers
    void onKeyPress (finalcut::FKeyEvent*) override;
    void onWheel (finalcut::FWheelEvent*) override;
    void onClose (finalcut::FCloseEvent*) override;

    // Callback methods
    void cb_next();
    void cb_back();

  private:
    // Constants
    static constexpr auto UNDEFINED = static_cast<FColor>(-2);

    // Method
    void adjustSize() override;
    void draw() override;

    // Data members
    FColor bgcolor{UNDEFINED};
    finalcut::FButton next_button{"&Next >", this};
    finalcut::FButton back_button{"< &Back", this};
};

//----------------------------------------------------------------------
AttribDlg::AttribDlg (finalcut::FWidget* parent)
  : finalcut::FDialog{parent}
{
  next_button.setGeometry ( FPoint{int(getWidth()) - 13, int(getHeight()) - 4}
                          , FSize{10, 1} );
  next_button.addAccelerator (fc::Fkey_right);
  back_button.setGeometry ( FPoint{int(getWidth()) - 25, int(getHeight()) - 4}
                          , FSize{10, 1} );
  back_button.addAccelerator (fc::Fkey_left);

  // Add function callbacks
  next_button.addCallback
  (
    "clicked",
    this, &AttribDlg::cb_next
  );

  back_button.addCallback
  (
    "clicked",
    this, &AttribDlg::cb_back
  );
}

//----------------------------------------------------------------------
FColor AttribDlg::getBGColor() const
{
  return bgcolor;
}

//----------------------------------------------------------------------
void AttribDlg::onKeyPress (finalcut::FKeyEvent* ev)
{
  if ( ! ev )
    return;

  if ( ev->key() == 'q' )
  {
    close();
    ev->accept();
  }
  else
    finalcut::FDialog::onKeyPress(ev);
}

//----------------------------------------------------------------------
void AttribDlg::onWheel (finalcut::FWheelEvent* ev)
{
  const int wheel = ev->getWheel();

  if ( wheel == fc::WheelUp )
    cb_next();
  else if ( wheel == fc::WheelDown )
    cb_back();
}

//----------------------------------------------------------------------
void AttribDlg::onClose (finalcut::FCloseEvent* ev)
{
  finalcut::FApplication::closeConfirmationDialog (this, ev);
}

//----------------------------------------------------------------------
void AttribDlg::cb_next()
{
  if ( finalcut::FTerm::isMonochron() )
    return;

  if ( bgcolor == FColor(finalcut::FTerm::getMaxColor() - 1) )
    bgcolor = fc::Default;
  else if ( bgcolor == fc::Default )
    bgcolor = 0;
  else
    bgcolor++;

  redraw();
}

//----------------------------------------------------------------------
void AttribDlg::cb_back()
{
  if ( finalcut::FTerm::isMonochron() )
    return;

  if ( bgcolor == 0 )
    bgcolor = fc::Default;
  else if ( bgcolor == fc::Default )
    bgcolor = FColor(finalcut::FTerm::getMaxColor() - 1);
  else
    bgcolor--;

  redraw();
}

//----------------------------------------------------------------------
void AttribDlg::adjustSize()
{
  auto x = int((getDesktopWidth() - getWidth()) / 2);
  auto y = int((getDesktopHeight() - getHeight()) / 2) + 1;

  if ( x < 1 )
    x = 1;

  if ( y < 1 )
    y = 1;

  setGeometry(FPoint{x, y}, FSize{69, 21}, false);
  next_button.setGeometry ( FPoint{int(getWidth()) - 13, int(getHeight()) - 4}
                          , FSize{10, 1}, false );
  back_button.setGeometry ( FPoint{int(getWidth()) - 25, int(getHeight()) - 4}
                          , FSize{10, 1}, false );
  finalcut::FDialog::adjustSize();
}

//----------------------------------------------------------------------
void AttribDlg::draw()
{
  if ( bgcolor == UNDEFINED )
  {
    // Get the color after initializing the color theme in show()
    if ( finalcut::FTerm::isMonochron() )
      bgcolor = fc::Default;
    else
      bgcolor = getColorTheme()->label_bg;

    // Get the terminal type after the terminal detection in show()
    FDialog::setText ( "A terminal attributes test ("
                     + finalcut::FString{finalcut::FTerm::getTermType()}
                     + ")");
  }

  FDialog::draw();
}


//----------------------------------------------------------------------
// class AttribDemo
//----------------------------------------------------------------------

class AttribDemo final : public finalcut::FWidget
{
  public:
    // Constructor
    explicit AttribDemo (FWidget* = nullptr);

    // Destructor
    ~AttribDemo() override = default;

    // Event handler
    void onWheel (finalcut::FWheelEvent* ev) override
    {
      auto p = static_cast<AttribDlg*>(getParentWidget());

      if ( p )
        p->onWheel(ev);
    }

  private:
    // Methods
    void printColorLine();
    void printAltCharset();
    void printDim();
    void printNormal();
    void printBold();
    void printBoldDim();
    void printItalic();
    void printUnderline();
    void printDblUnderline();
    void printCrossesOut();
    void printBlink();
    void printReverse();
    void printStandout();
    void printInvisible();
    void printProtected();
    void draw() override;

    // Data member
    FColor last_color{1};
};

//----------------------------------------------------------------------
AttribDemo::AttribDemo (finalcut::FWidget* parent)
  : finalcut::FWidget{parent}
{
  unsetFocusable();
}

//----------------------------------------------------------------------
void AttribDemo::printColorLine()
{
  const auto& parent = static_cast<AttribDlg*>(getParent());

  for (FColor color{0}; color < last_color; color++)
  {
    print() << FColorPair{color, parent->getBGColor()} << " # ";
  }
}

//----------------------------------------------------------------------
void AttribDemo::printAltCharset()
{
  const auto& wc = getColorTheme();
  const auto& parent = static_cast<AttribDlg*>(getParent());

  if ( ! finalcut::FTerm::isMonochron() )
    setColor (wc->label_fg, wc->label_bg);

  print() << FPoint{1, 1} << "alternate charset: ";

  if ( parent->getBGColor() == fc::Default )
  {
    setColor (fc::Default, fc::Default);
  }
  else
  {
    if ( (parent->getBGColor() <= 8)
      || (parent->getBGColor() >= 16 && parent->getBGColor() <= 231
        && (parent->getBGColor() - 16) % 36 <= 17)
      || (parent->getBGColor() >= 232 && parent->getBGColor() <= 243) )
      setColor (fc::White, parent->getBGColor());
    else
      setColor (fc::Black, parent->getBGColor());
  }

  setAltCharset();
  print("`abcdefghijklmnopqrstuvwxyz{|}~");
  unsetAltCharset();
  print("                 ");
}

//----------------------------------------------------------------------
void AttribDemo::printDim()
{
  print("              Dim: ");
  setDim();
  printColorLine();
  unsetDim();
}

//----------------------------------------------------------------------
void AttribDemo::printNormal()
{
  print("           Normal: ");
  setNormal();
  printColorLine();
}

//----------------------------------------------------------------------
void AttribDemo::printBold()
{
  print("             Bold: ");
  setBold();
  printColorLine();
  unsetBold();
}

//----------------------------------------------------------------------
void AttribDemo::printBoldDim()
{
  print("         Bold+Dim: ");
  setBold();
  setDim();
  printColorLine();
  unsetDim();
  unsetBold();
}

//----------------------------------------------------------------------
void AttribDemo::printItalic()
{
  print("           Italic: ");
  setItalic();
  printColorLine();
  unsetItalic();
}

//----------------------------------------------------------------------
void AttribDemo::printUnderline()
{
  print("        Underline: ");
  setUnderline();
  printColorLine();
  unsetUnderline();
}

//----------------------------------------------------------------------
void AttribDemo::printDblUnderline()
{
  print(" Double underline: ");
  setDoubleUnderline();
  printColorLine();
  unsetDoubleUnderline();
}

//----------------------------------------------------------------------
void AttribDemo::printCrossesOut()
{
  print("      Crossed-out: ");
  setCrossedOut();
  printColorLine();
  unsetCrossedOut();
}

//----------------------------------------------------------------------
void AttribDemo::printBlink()
{
  print("            Blink: ");
  setBlink();
  printColorLine();
  unsetBlink();
}

//----------------------------------------------------------------------
void AttribDemo::printReverse()
{
  print("          Reverse: ");
  setReverse();
  printColorLine();
  unsetReverse();
}

//----------------------------------------------------------------------
void AttribDemo::printStandout()
{
  print("         Standout: ");
  setStandout();
  printColorLine();
  unsetStandout();
}

//----------------------------------------------------------------------
void AttribDemo::printInvisible()
{
  print("        Invisible: ");
  setInvisible();
  printColorLine();
  unsetInvisible();
}

//----------------------------------------------------------------------
void AttribDemo::printProtected()
{
  print("        Protected: ");
  setProtected();
  printColorLine();
  unsetProtected();
}

//----------------------------------------------------------------------
void AttribDemo::draw()
{
  const auto& wc = getColorTheme();
  last_color = FColor(finalcut::FTerm::getMaxColor());

  if ( finalcut::FTerm::isMonochron() )
    last_color = 1;
  else if ( last_color > 16 )
    last_color = 16;

  // test alternate character set
  printAltCharset();

  const std::vector<std::function<void()> > effect
  {
    [this] { printNormal(); },
    [this] { printBold(); },
    [this] { printBoldDim(); },
    [this] { printItalic(); },
    [this] { printUnderline(); },
    [this] { printDblUnderline(); },
    [this] { printCrossesOut(); },
    [this] { printBlink(); },
    [this] { printReverse(); },
    [this] { printStandout(); },
    [this] { printInvisible(); },
    [this] { printProtected(); },
  };

  for (std::size_t y{0}; y < getParentWidget()->getHeight() - 7; y++)
  {
    print() << FPoint{1, 2 + int(y)};

    if ( ! finalcut::FTerm::isMonochron() )
      setColor (wc->label_fg, wc->label_bg);

    if ( y < effect.size() )
      effect[y]();
  }

  if ( ! finalcut::FTerm::isMonochron() )
    setColor(wc->label_fg, wc->label_bg);

  print() << FPoint{1, 15};
  const FColor bg = static_cast<AttribDlg*>(getParent())->getBGColor();
  print (" Background color:");

  if ( bg == fc::Default )
    print (" default");
  else
    printf ( " %d", bg);

  print() << FPoint{16, 17} << "Change background color ->";
}


//----------------------------------------------------------------------
//                               main part
//----------------------------------------------------------------------
int main (int argc, char* argv[])
{
  // Create the application object
  finalcut::FApplication app {argc, argv};

  // Create a dialog box object.
  // This object will be automatically deleted by
  // the parent object "app" (FObject destructor).
  AttribDlg dialog{&app};
  dialog.setSize (FSize{69, 21});
  dialog.setShadow();  // Instead of the transparent window shadow

  // Create the attribute demo widget as a child object from the dialog
  AttribDemo demo(&dialog);
  demo.setGeometry (FPoint{1, 1}, FSize{67, 19});

  // Set the dialog object as main widget
  finalcut::FWidget::setMainWidget(&dialog);

  // Show and start the application
  dialog.show();
  return app.exec();
}
