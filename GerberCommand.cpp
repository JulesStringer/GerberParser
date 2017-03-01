#include "GerberCommand.h"
#include "GerberParser.h"
#include "GerberSegment.h"
#include "GerberInterpolate.h"
#include "GerberState.h"
#include "GerberEndSegment.h"
#include "GerberMove.h"
#include "GerberFlash.h"
#include "GerberAperatureBlock.h"
#include "GerberStepRepeat.h"

CGerberCommand::CGerberCommand()
{
}
CGerberCommand::CGerberCommand(CGerberState* pState)
	: m_pState(pState)
{}
CGerberCommand::~CGerberCommand()
{
}
CGerberCommand* CGerberCommand::Create(CGerberParser* pParser, CGerberState* pState)
{
	CInputStream* pStream = pParser->Stream();
	unsigned int nCode = 0;
	double X = 0;
	double Y = 0;
	double I = 0;
	double J = 0;
	for (char ch = pStream->GetChar(); ch != '*' && !pStream->AtEOF(); ch = pStream->GetChar())
	{
		switch (ch)
		{
		case 'G':
			// Get Code number at least 2 digits and no more than 10
			nCode = pStream->GetInt();
			// Act on code
			// 1, 2, 3, 36, 37, 74, 75 create appropriate command
			// 4, skip to end
			// else error - ignore
			switch (nCode)
			{
			case 1:
				pState->SetInterpolation(INTERPOLATION_LINEAR);
				break;
			case 2:
				pState->SetInterpolation(INTERPOLATION_CLOCKWISE);
				break;
			case 3:
				pState->SetInterpolation(INTERPOLATION_ANTICLOCK);
				break;
			case 4:
				// skip to *
				// Parse comment to determine if a layer change is required
				//
				{
					std::string strComment;
					while (ch != '*')
					{
						ch = pStream->GetChar();
						if (ch != '*')
						{
							strComment += ch;
						}
					}
					CLayer* pLayer = pParser->MatchingLayer(strComment.c_str());
					if (pLayer)
					{
						pState->SetLayer(pLayer);
					}
					else
					{
						pParser->PartCheck(strComment.c_str(), pState);
					}
				}
//				pStream->SkipToEndBlock();
				break;
			case 36:
				{
					// Start region - create region command add following commands to it.
				    pStream->SkipToEndBlock();
					CGerberSegment* pSegment = new CGerberSegment();
					pSegment->Parse(pParser, new CGerberState(pState));
					return pSegment;
				}
				break;
			case 37:
				// End Region - add region command to commands and cancel region
				// Should this be handled by CGerberSegment
				return new CGerberEndSegment();
			case 74:
				pState->SetMultiSegment(false);
				break;
			case 75:
				pState->SetMultiSegment(true);
				break;
			}
			break;
		case 'D':
			// Get Code number at least 2 digits and no more than 10
			nCode = pStream->GetInt();
			pStream->SkipToEndBlock();
			// Act on code
			switch (nCode)
			{
			case 1:
				// Interpolate - action depends on GCODE
			    return new CGerberInterpolate(X, Y, I, J, new CGerberState(pState));
			case 2:
				// Move to X, Y
				return new CGerberMove(X, Y, new CGerberState(pState));
			case 3:
				// Flash current aperature at X, Y
				{
					CGerberAperature* pAperature = pState->GetAperature();
					if (!pAperature)
					{
						throw "D03 command when no aperature set";
					}
					return new CGerberFlash(X, Y, pAperature, new CGerberState(pState));
				}
				break;
			default:
				// Set current aperature
				// Get aperature from parser
				{
					CGerberAperature* pAperature = pParser->GetAperature(nCode);
					if (!pAperature)
					{
						throw "Aperature not defined before reference";
					}
					pState->SetAperature(pAperature);
				}
				break;
			}
			break;
		case 'M':
			// Get Code number at least 2 digits and no more than 10
			nCode = pStream->GetInt();
			// 2 indicates end of file - others ignore
			break;
		case 'X':
			X = pStream->GetXCoordinate();
			break;
		case 'Y':
			Y = pStream->GetYCoordinate();
			break;
		case 'I':
			I = pStream->GetSignedDouble();
			break;
		case 'J':
			J = pStream->GetSignedDouble();
			break;
		case '%':
			{
				std::string str = pStream->GetExtendedCommand();
				// Extended commands
				// Get data blocks until *% is encountered
				// Commands are
				if (str == "FS")
				{
					//  FS Coordinate format
					//  Form is
					//  FSLAXndYnd*%
					//  n is number of digits before decimal place in X or Y
					//  d is number of digits after decimal place in X or Y
					// Values are set on pStream
					pStream->GetCoordinateFormat();
				}
				else if (str == "MO")
				{
					//  MO Unit - mm or inch
					//  either
					//  MOIN*%  - inches
					//  MOMM*%  - millimetres
					// Values are set on pStream
					std::string str = pStream->GetExtendedCommand();
					if (str == "IN")
					{
						pStream->SetUnits(25.4);
					}
					else if (str == "MM")
					{
						pStream->SetUnits(1.0);
					}
					else
					{
						throw "Units must be IN or MM";
					}
					pStream->SkipToEndExtended();
				}
				else if (str == "AD")
				{
					//  AD Aperture definition
					//  Followed by D then unsigned integer DCODE (>=10)
					//  Followed by standard primitive or macro name followed by parameters
					//  parameters are doubles with decimal point 
					//  parameters are separated by X
					//  parameters are terminated by *%
					// Create aperture command and parse internally
					// Store on parser
					ch = pStream->GetChar();
					if (ch != 'D')
					{
						throw "AD Command must be followed by D";
					}
					unsigned int nCode = pStream->GetInt();
					ch = pStream->GetChar();
					std::string strName;
					while (!pStream->AtEOF() && ch != ',' && ch != '*')
					{
						strName += ch;
						ch = pStream->GetChar();
					}
					CGerberAperature* pAperature = CGerberAperature::Create(strName.c_str(), pParser);
					if (pAperature)
					{
						pAperature->Parse(pParser);
						pParser->AddAperature(nCode, pAperature);
					}
					pStream->SkipToEndExtended();
				}
				else if (str == "AM")
				{
					//  AM Aperture macro
					//  Name is everything following up to *
					std::string strName = "";
					ch = pStream->GetChar();
					while (ch != '*')
					{
						strName += ch;
						ch = pStream->GetChar();
					}
					//  Then primitives including arguments, each primitive ending in *
					//  arguments reference are expressions using numerics and passed parameters prefixed by $
					//  Parameters can also be defined in macro before they are referenced.
					//  Final one ends *%
					// Create macro command and parse internally
					// Store on parser
					CGerberMacro* pMacro = new CGerberMacro();
					pMacro->Parse(pParser);
					pParser->AddMacro(strName.c_str(), pMacro);
				}
				else if (str == "AB")
				{
					//  AB Aperture block
					//  Followed by D then unsigned integer DCODE (>=10)
					//  then *%
					ch = pStream->GetChar();
					if (ch != 'D')
					{
						throw "AD Command must be followed by D";
					}
					unsigned int nCode = pStream->GetInt();
					pStream->SkipToEndExtended();
					//  Then commands comprising the block
					//  then terminated by %AB*%
					// Create Block command and parse up
					// Block derived from aperture
					// Store on parser
					CGerberAperatureBlock* pBlock = new CGerberAperatureBlock();
					pBlock->Parse(pParser);
					pParser->AddAperature(nCode, pBlock);
				}
				else if (str == "LP")
				{
					//  LP Load polarity (dark / clear)
					//  LPD*% - dark
					//  LPC*% - clear
					ch = pStream->GetChar();
					if (ch == 'D')
					{
						pState->SetDark(true);
					}
					else if (ch == 'C')
					{
						pState->SetDark(false);
					}
					pStream->SkipToEndExtended();
				}
				else if (str == "LM")
				{
					//  LM Load mirror
					//  LMN*%     no mirror
					//  LMX*%
					//  LMY*%
					//  LMXY*%
					ch = pStream->GetChar();
					if (ch == 'N')
					{
						pState->SetXMirror(false);
						pState->SetYMirror(false);
					}
					else if (ch == 'X')
					{
						pState->SetXMirror(true);
						ch = pStream->GetChar();
					}
					if (ch == 'Y')
					{
						pState->SetYMirror(true);
					}
					pStream->SkipToEndExtended();
				}
				else if (str == "LR")
				{
					//  LR Load rotation
					// LR<Rotation>*%
					// <Rotation> is in decimal degrees and may be negative, +ve angle is counterclockwise
					double d = pStream->GetSignedDouble();
					pState->SetRotation(d);
					pStream->SkipToEndExtended();
				}
				else if (str == "LS")
				{
					//  LS Load Scale
					//  LS<scale>*%
					//  <scale> is scale factor with 1.0 meaning no scaling
					double d = pStream->GetSignedDouble();
					pState->SetScale(d);
					pStream->SkipToEndExtended();
				}
				else if (str == "SR")
				{
					//  SR Step and repeat
					//  SR[X<repeats>Y<repeats>I<step>J<step>]*%
					//  SR*% means end block accumulation
					//  Create block object
					//  Parse up to %SR*%
					//  Action is to repeat block
					CGerberStepRepeat* pStepRepeat = new CGerberStepRepeat();
					pStepRepeat->Parse(pParser, new CGerberState(pState));
					return pStepRepeat;
				}
				else
				{
					pStream->SkipToEndExtended();
				}
			}
			break;
		}
	}
	return NULL;
}
void CGerberCommand::Render(CGerberRender* pRender, double dDeflate)
{
	throw "CGerberCommand::Render not implemented in derived class, unimplemented base class implementation called";
}
void CGerberCommand::Inflate(CPart* pPart)
{
	throw "CGerberCommand::Inflate not implemented in derived class, unimplemented base class implementation called";
}
void CGerberCommand::Inflate(CRectangle* pRect)
{
	throw "CGerberCommand::Inflate not implemented in derived class, unimplemented base class implementation called";
}
bool CGerberCommand::MatchesLayer(CLayer* pLayer)
{
	if (m_pState)
	{
		return m_pState->GetLayer() == pLayer;
	}
	return false;
}
