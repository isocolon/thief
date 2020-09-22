#include "thief.h"

BOOL CALLBACK MoveInputBoxWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hMove, hSmart, hTrue, hObserve, hExam, hPlay;
	char cTmp [2048];
	int  nI;

	switch(iMsg)
	{
		case WM_INITDIALOG :
			if(! STATE_EnterDialogBox())
			{
				EndDialog(hwnd, FALSE);
				return FALSE;
			}

			hMove    = GetDlgItem(hwnd, IDD_MOVEINPUT_1);
			hSmart   = GetDlgItem(hwnd, IDD_MOVEINPUT_4);
			hTrue    = GetDlgItem(hwnd, IDD_MOVEINPUT_5);
			hObserve = GetDlgItem(hwnd, IDD_MOVEINPUT_11);
			hExam    = GetDlgItem(hwnd, IDD_MOVEINPUT_12);
			hPlay    = GetDlgItem(hwnd, IDD_MOVEINPUT_13);

			if((User.nMoveType != DRAG_MOVE) &&
					(User.nMoveType != CLICK_MOVE) &&
					(User.nMoveType != INVISIBLE_MOVE))
			{
				User.nMoveType = DRAG_MOVE;
			}

			SMARTMOVE_Adjust();
			TRUEPREMOVE_Adjust();

			SendMessage(hMove, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Drag Move");
			SendMessage(hMove, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Click Move");
			SendMessage(hMove, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Invisible Move");
			SendMessage(hMove, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(User.nMoveType)
			{
				case DRAG_MOVE :
					strcpy(cTmp, "Drag Move");
					break;

				case CLICK_MOVE :
					strcpy(cTmp, "Click Move");
					break;

				case INVISIBLE_MOVE :
					strcpy(cTmp, "Invisible Move");
					break;

				default :
					User.nMoveType = DRAG_MOVE;
					strcpy(cTmp, "Drag Move");
					break;
			}

			if(SendMessage(hMove, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hMove, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hMove, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}
			if(User.nMoveType == CLICK_MOVE)
			{
				// if click mode type disable smart move on button up checkbox
				EnableWindow(GetDlgItem(hwnd, IDD_MOVEINPUT_15), 0);
			}

			SendMessage(hSmart, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "Click on Piece");
			SendMessage(hSmart, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Click on Destination Square");
			SendMessage(hSmart, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "Click on Piece and Destination Square");
			SendMessage(hSmart, CB_ADDSTRING, 0, (LPARAM) cTmp);

			if(User.bSmartMove1)
			{
				if(User.bSmartMove2)
				{
					strcpy(cTmp, "Click on Piece and Destination Square");
				}
				else
				{
					strcpy(cTmp, "Click on Piece");
				}
			}
			else if(User.bSmartMove2)
			{
				strcpy(cTmp, "Click on Destination Square");
			}
			else
			{
				strcpy(cTmp, "Click on Piece and Destination Square");
			}

			if(SendMessage(hSmart, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hSmart, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hSmart, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendMessage(hTrue, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "None");
			SendMessage(hTrue, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "True Premove (Thief Style)");
			SendMessage(hTrue, CB_ADDSTRING, 0, (LPARAM) cTmp);

			strcpy(cTmp, "True Premove (Lightning Rulez Style)");
			SendMessage(hTrue, CB_ADDSTRING, 0, (LPARAM) cTmp);

			switch(User.nTruePremove)
			{
				case PREMOVE_NONE :
					strcpy(cTmp, "None");
					break;

				case PREMOVE_THIEF :
					strcpy(cTmp, "True Premove (Thief Style)");
					break;

				case PREMOVE_LR :
					strcpy(cTmp, "True Premove (Lightning Rulez Style)");
					break;

				default :
					User.bTruePremove = 0;
					User.nTruePremove = PREMOVE_NONE;
					strcpy(cTmp, "None");
					break;
			}

			if(SendMessage(hTrue, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hTrue, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hTrue, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_2,  BM_SETCHECK, User.bAutoCenterDragPiece ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_3,  BM_SETCHECK, User.bSmartMove           ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_6,  BM_SETCHECK, User.bIllegalTPBeep       ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_7,  BM_SETCHECK, User.bRightMousePlay      ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_8,  BM_SETCHECK, User.bRightMousePartner   ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_9,  BM_SETCHECK, User.bTestLegalPlay       ? BST_CHECKED : BST_UNCHECKED, 0);
			SendDlgItemMessage(hwnd, IDD_MOVEINPUT_10, BM_SETCHECK, User.bTestLegalPartner    ? BST_CHECKED : BST_UNCHECKED, 0);

			SendMessage(hObserve, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "None");
			SendMessage(hObserve, CB_ADDSTRING, 0, (LPARAM) cTmp);

			for(nI = 0 ; nI < MAX_ANIMATE_SPEED ; nI++)
			{
				sprintf(cTmp, "%d Milliseconds", (nI + 1) * 5);
				SendMessage(hObserve, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			SendMessage(hExam, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "None");
			SendMessage(hExam, CB_ADDSTRING, 0, (LPARAM) cTmp);

			for(nI = 0 ; nI < MAX_ANIMATE_SPEED ; nI++)
			{
				sprintf(cTmp, "%d Milliseconds", (nI + 1) * 5);
				SendMessage(hExam, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			SendMessage(hPlay, CB_RESETCONTENT, 0, 0);

			strcpy(cTmp, "None");
			SendMessage(hPlay, CB_ADDSTRING, 0, (LPARAM) cTmp);

			for(nI = 0 ; nI < MAX_ANIMATE_SPEED ; nI++)
			{
				sprintf(cTmp, "%d Milliseconds", (nI + 1) * 5);
				SendMessage(hPlay, CB_ADDSTRING, 0, (LPARAM) cTmp);
			}

			if(User.nAnimateSpeedObserve < 0)
			{
				User.nAnimateSpeedObserve = 0;
			}
			else if(User.nAnimateSpeedObserve >= MAX_ANIMATE_SPEED)
			{
				User.nAnimateSpeedObserve = MAX_ANIMATE_SPEED - 1;
			}

			if(User.nAnimateSpeedExam < 0)
			{
				User.nAnimateSpeedExam = 0;
			}
			else if(User.nAnimateSpeedExam >= MAX_ANIMATE_SPEED)
			{
				User.nAnimateSpeedExam = MAX_ANIMATE_SPEED - 1;
			}

			if(User.nAnimateSpeedPlay < 0)
			{
				User.nAnimateSpeedPlay = 0;
			}
			else if(User.nAnimateSpeedPlay >= MAX_ANIMATE_SPEED)
			{
				User.nAnimateSpeedPlay = MAX_ANIMATE_SPEED - 1;
			}

			if(User.bAnimateObserve)
			{
				sprintf(cTmp, "%d Milliseconds", (User.nAnimateSpeedObserve + 1) * 5);
			}
			else
			{
				strcpy(cTmp, "None");
			}

			if(SendMessage(hObserve, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hObserve, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hObserve, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			if(User.bAnimateExam)
			{
				sprintf(cTmp, "%d Milliseconds", (User.nAnimateSpeedExam + 1) * 5);
			}
			else
			{
				strcpy(cTmp, "None");
			}

			if(SendMessage(hExam, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hExam, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hExam, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			if(User.bAnimatePlay)
			{
				sprintf(cTmp, "%d Milliseconds", (User.nAnimateSpeedPlay + 1) * 5);
			}
			else
			{
				strcpy(cTmp, "None");
			}

			if(SendMessage(hPlay, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
			{
				SendMessage(hPlay, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
				SendMessage(hPlay, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
			}

			TOOLBOX_CenterWindow(hwnd, GetWindow(hwnd, GW_OWNER));
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK :
					nI = SendMessage(hMove, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.nMoveType = DRAG_MOVE;
					}
					else
					{
						switch(nI)
						{
							case 0 :
								User.nMoveType = DRAG_MOVE;
								break;
							case 1 :
								User.nMoveType = CLICK_MOVE;
								break;
							case 2 :
								User.nMoveType = INVISIBLE_MOVE;
								break;
							default :
								User.nMoveType = DRAG_MOVE;
								break;
						}
					}

					nI = SendMessage(hSmart, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.bSmartMove1 = 0;
						User.bSmartMove2 = 0;
					}
					else
					{
						switch(nI)
						{
							case 0 :
								User.bSmartMove1 = 1;
								User.bSmartMove2 = 0;
								break;

							case 1 :
								User.bSmartMove1 = 0;
								User.bSmartMove2 = 1;
								break;

							case 2 :
								User.bSmartMove1 = 1;
								User.bSmartMove2 = 1;
								break;

							default :
								User.bSmartMove1 = 1;
								User.bSmartMove2 = 1;
								break;
						}
					}

					nI = SendMessage(hTrue, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.nTruePremove = PREMOVE_NONE;
						User.bTruePremove = 0;
					}
					else
					{
						switch(nI)
						{
							case 0 :
								User.nTruePremove = PREMOVE_NONE;
								User.bTruePremove = 0;
								break;

							case 1 :
								User.nTruePremove = PREMOVE_THIEF;
								User.bTruePremove = 1;
								break;

							case 2 :
								User.nTruePremove = PREMOVE_LR;
								User.bTruePremove = 1;
								break;

							default :
								User.nTruePremove = PREMOVE_NONE;
								User.bTruePremove = 0;
								break;
						}
					}

					User.bAutoCenterDragPiece = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_2,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bSmartMove           = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_3,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bIllegalTPBeep       = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_6,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bRightMousePlay      = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_7,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bRightMousePartner   = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_8,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bTestLegalPlay       = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_9,  BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);
					User.bTestLegalPartner    = ((SendDlgItemMessage(hwnd, IDD_MOVEINPUT_10, BM_GETSTATE, 0, 0) == BST_CHECKED) ? 1 : 0);

					nI = SendMessage(hObserve, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.bAnimateObserve      = 1;
						User.nAnimateSpeedObserve = 0;
					}
					else
					{
						if(nI == 0)
						{
							User.bAnimateObserve      = 0;
							User.nAnimateSpeedObserve = 0;
						}
						else
						{
							User.bAnimateObserve      = 1;
							User.nAnimateSpeedObserve = (nI - 1);
						}
					}

					nI = SendMessage(hExam, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.bAnimateExam      = 1;
						User.nAnimateSpeedExam = 0;
					}
					else
					{
						if(nI == 0)
						{
							User.bAnimateExam      = 0;
							User.nAnimateSpeedExam = 0;
						}
						else
						{
							User.bAnimateExam      = 1;
							User.nAnimateSpeedExam = (nI - 1);
						}
					}

					nI = SendMessage(hPlay, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
					if(nI == CB_ERR)
					{
						User.bAnimatePlay      = 1;
						User.nAnimateSpeedPlay = 0;
					}
					else
					{
						if(nI == 0)
						{
							User.bAnimatePlay      = 0;
							User.nAnimateSpeedPlay = 0;
						}
						else
						{
							User.bAnimatePlay      = 1;
							User.nAnimateSpeedPlay = (nI - 1);
						}
					}

					EndDialog(hwnd, TRUE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDCANCEL :
					EndDialog(hwnd, FALSE);
					STATE_LeaveDialogBox();
					return TRUE;

				case IDD_MOVEINPUT_1 :
					if(HIWORD(wParam) == CBN_SELCHANGE)
					{
						nI = SendMessage(hMove, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
						if(nI == CB_ERR)
						{
							User.nMoveType = DRAG_MOVE;
						}
						else
						{
							if(nI == 1)
							{
								// click move type is incompatible with smartmove on Button Up
								EnableWindow(GetDlgItem(hwnd, IDD_MOVEINPUT_15), 0);
								SendDlgItemMessage(hwnd, IDD_MOVEINPUT_15, BM_SETCHECK,
												   BST_UNCHECKED, 0);
							}
							else
							{
								EnableWindow(GetDlgItem(hwnd, IDD_MOVEINPUT_15), 1);
							}
						}
					}
					return TRUE;

				case IDD_MOVEINPUT_14 :
					strcpy(cTmp, "Drag Move");
					if(SendMessage(hMove, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hMove, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hMove, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, "Click on Piece and Destination Square");
					if(SendMessage(hSmart, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hSmart, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hSmart, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, "None");
					if(SendMessage(hTrue, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hTrue, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hTrue, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_2,  BM_SETCHECK, BST_CHECKED,   0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_3,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_6,  BM_SETCHECK, BST_CHECKED,   0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_7,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_8,  BM_SETCHECK, BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_9,  BM_SETCHECK, BST_CHECKED,   0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_10, BM_SETCHECK, BST_CHECKED,   0);
					SendDlgItemMessage(hwnd, IDD_MOVEINPUT_15, BM_SETCHECK, BST_UNCHECKED, 0);
					EnableWindow(GetDlgItem(hwnd, IDD_MOVEINPUT_15), 1);

					strcpy(cTmp, "5 Milliseconds");
					if(SendMessage(hObserve, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hObserve, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hObserve, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, "5 Milliseconds");
					if(SendMessage(hExam, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hExam, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hExam, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}

					strcpy(cTmp, "None");
					if(SendMessage(hPlay, CB_SELECTSTRING, (WPARAM) -1, (LPARAM) cTmp) == CB_ERR)
					{
						SendMessage(hPlay, CB_SETCURSEL, (WPARAM) -1, (LPARAM) 0);
						SendMessage(hPlay, WM_SETTEXT, (WPARAM) 0, (LPARAM) cTmp);
					}
					return TRUE;
			}
			break;
	}
	return FALSE;
}
