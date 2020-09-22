#include "thief.h"

void EAR_Append(void)
{
	char *cStr [MAX_EAR] = { "Sit",
							 "Go",
							 "Fast",
							 "Time",
							 "++",
							 "----",
							 "+Q",
							 "-Q",
							 "+R",
							 "-R",
							 "+N",
							 "-N",
							 "+B",
							 "-B",
							 "+P",
							 "-P",
							 "OK",
							 "OK now",
							 "Hard",
							 "Coming",
							 "Maybe",
							 "I sit",
							 "Mates me",
							 "Mates him",
							 "I dead",
							 "Opp dead",
							 "Yes",
							 "No",
							 "Tell Me Go",
							 "Sit Until I Tell U Go",
							 "Keep Checking",
							 "Nevermind",
							 "+",
							 "+++",
							 "-",
							 "- -",
							 "- - -",
							 "Pawn",
							 "Bishop",
							 "Knight",
							 "Rook",
							 "Queen",
							 "I lose...",
							 "Can I go?",
							 "Take",
							 "What u want?",
							 "No!",
							 "I Mated",
							 "Opp Mated",
							 "We up",
							 "We down",
							 "U get",
							 "He gets",
							 "Watchout",
							 "Feed me",
							 "Lag",
							 "Lag cheating fuckers",
							 "Mother fucking lag",
							 "Bitch is dead",
							 "Bite me",
							 "Cake",
							 "Eat shit",
							 "Fuck u",
							 "I finger focked",
							 "I finger focking",
							 "Move faster bitch",
							 "Son of a bitch",
							 "So easy",
							 "She dead",
							 "They suck"
						   };

	char *cWav [MAX_EAR] = { "SIT",
							 "GO",
							 "PLAYFAST",
							 "LOOKTIME",
							 "YESTRADE",
							 "NOTRADE",
							 "QGOOD",
							 "NOQ",
							 "RGOOD",
							 "NOR",
							 "NGOOD",
							 "NON",
							 "BGOOD",
							 "NOB",
							 "PGOOD",
							 "NOP",
							 "OK",
							 "OKNOW",
							 "HARD2GET",
							 "COMING",
							 "MAYBE",
							 "ISIT",
							 "MATESME",
							 "MATESHIM",
							 "IMDEAD",
							 "OPPMATED",
							 "YES",
							 "NO",
							 "TELLMEGO",
							 "TELLUGO",
							 "KEEPCHECKING",
							 "NEVERMIND",
							 "+",
							 "+++",
							 "-",
							 "-_-",
							 "-_-_-",
							 "PAWN",
							 "BISHOP",
							 "KNIGHT",
							 "ROOK",
							 "QUEEN",
							 "ILOSE",
							 "CANIGO",
							 "TAKE",
							 "WHATUWANT",
							 "NO",
							 "IMATED",
							 "OPPMATED",
							 "WEUP",
							 "WEDOWN",
							 "YOUGET",
							 "HEGETS",
							 "WATCHOUT",
							 "FEEDME",
							 "LAG",
							 "LAGCHEATINGFUCKERS",
							 "MOTHERLAG",
							 "BITCHDEAD",
							 "BITEME",
							 "CAKE",
							 "EATSHIT",
							 "FUKYOUMOTHER",
							 "IFINGERFOCKED",
							 "IFINGERFOCKING",
							 "MOVEFASTBITCH",
							 "SONOFABITCH",
							 "SOEASY",
							 "SHEDEAD",
							 "THEYSUCK"
						   };

	int nI, nJ, bFound, nEmpty;

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		bFound = 0;
		nEmpty = -1;

		for(nJ = 0 ; nJ < MAX_EAR ; nJ++)
		{
			if((strlen(Ears.cTell [nJ]) == 0) || (strlen(Ears.cName [nJ]) == 0))
			{
				if(nEmpty == -1)
				{
					nEmpty = nJ;
				}
			}
			else
			{
				if(stricmp(Ears.cTell [nJ], cStr [nI]) == 0)
				{
					bFound = 1;
					break;
				}
			}
		}

		if(! bFound)
		{
			if(nEmpty == -1)
			{
				break;
			}

			Ears.bBeep [nEmpty] = 0;

			strcpy(Ears.cTell [nEmpty], cStr [nI]);

			strcpy(Ears.cName [nEmpty], cWav [nI]);
			strcat(Ears.cName [nEmpty], WAV_EXT);

			TOOLBOX_AllTrim(Ears.cTell [nEmpty]);
			TOOLBOX_AllTrim(Ears.cName [nEmpty]);
		}
	}
}

void EAR_Init(void)
{
	EAR_OldFICSEAR();
}

void EAR_OldFICSEAR(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		Ears.bPlay [nI] = 0;
		Ears.bBeep [nI] = 0;
		strcpy(Ears.cTell [nI], "");
		strcpy(Ears.cName [nI], "");
		Ears.pData [nI] = NULL;
	}

	// initialize old ear default
	strcpy(Ears.cTell [0], "Sit");
	strcpy(Ears.cName [0], "SIT.WAV");

	strcpy(Ears.cTell [1], "Go");
	strcpy(Ears.cName [1], "GO.WAV");

	strcpy(Ears.cTell [2], "Fast");
	strcpy(Ears.cName [2], "PLAYFAST.WAV");

	strcpy(Ears.cTell [3], "Time");
	strcpy(Ears.cName [3], "LOOKTIME.WAV");

	strcpy(Ears.cTell [4], "++");
	strcpy(Ears.cName [4], "YESTRADE.WAV");

	strcpy(Ears.cTell [5], "----");
	strcpy(Ears.cName [5], "NOTRADE.WAV");

	strcpy(Ears.cTell [6], "+Q");
	strcpy(Ears.cName [6], "QGOOD.WAV");

	strcpy(Ears.cTell [7], "-Q");
	strcpy(Ears.cName [7], "NOQ.WAV");

	strcpy(Ears.cTell [8], "+R");
	strcpy(Ears.cName [8], "RGOOD.WAV");

	strcpy(Ears.cTell [9], "-R");
	strcpy(Ears.cName [9], "NOR.WAV");

	strcpy(Ears.cTell [10], "+N");
	strcpy(Ears.cName [10], "NGOOD.WAV");

	strcpy(Ears.cTell [11], "-N");
	strcpy(Ears.cName [11], "NON.WAV");

	strcpy(Ears.cTell [12], "+B");
	strcpy(Ears.cName [12], "BGOOD.WAV");

	strcpy(Ears.cTell [13], "-B");
	strcpy(Ears.cName [13], "NOB.WAV");

	strcpy(Ears.cTell [14], "+P");
	strcpy(Ears.cName [14], "PGOOD.WAV");

	strcpy(Ears.cTell [15], "-P");
	strcpy(Ears.cName [15], "NOP.WAV");

	strcpy(Ears.cTell [16], "OK");
	strcpy(Ears.cName [16], "OK.WAV");

	strcpy(Ears.cTell [17], "OK now");
	strcpy(Ears.cName [17], "OKNOW.WAV");

	strcpy(Ears.cTell [18], "Hard");
	strcpy(Ears.cName [18], "HARD2GET.WAV");

	strcpy(Ears.cTell [19], "Coming");
	strcpy(Ears.cName [19], "COMING.WAV");

	strcpy(Ears.cTell [20], "Maybe");
	strcpy(Ears.cName [20], "MAYBE.WAV");

	strcpy(Ears.cTell [21], "I sit");
	strcpy(Ears.cName [21], "ISIT.WAV");

	strcpy(Ears.cTell [22], "Mates me");
	strcpy(Ears.cName [22], "MATESME.WAV");

	strcpy(Ears.cTell [23], "Mates him");
	strcpy(Ears.cName [23], "MATESHIM.WAV");

	strcpy(Ears.cTell [24], "I dead");
	strcpy(Ears.cName [24], "IMDEAD.WAV");

	strcpy(Ears.cTell [25], "Opp dead");
	strcpy(Ears.cName [25], "OPPMATED.WAV");

	strcpy(Ears.cTell [26], "Yes");
	strcpy(Ears.cName [26], "YES.WAV");

	strcpy(Ears.cTell [27], "No");
	strcpy(Ears.cName [27], "NO.WAV");

	strcpy(Ears.cTell [28], "Tell Me Go");
	strcpy(Ears.cName [28], "TELLMEGO.WAV");

	strcpy(Ears.cTell [29], "Sit Until I Tell U Go");
	strcpy(Ears.cName [29], "TELLUGO.WAV");

	strcpy(Ears.cTell [30], "Keep Checking");
	strcpy(Ears.cName [30], "KEEPCHECKING.WAV");

	strcpy(Ears.cTell [31], "Nevermind");
	strcpy(Ears.cName [31], "NEVERMIND.WAV");

	// include new FICS ear default
	strcpy(Ears.cTell [32], "+");
	strcpy(Ears.cName [32], "+.WAV");

	strcpy(Ears.cTell [33], "+++");
	strcpy(Ears.cName [33], "+++.WAV");

	strcpy(Ears.cTell [34], "-");
	strcpy(Ears.cName [34], "-.WAV");

	strcpy(Ears.cTell [35], "- -");
	strcpy(Ears.cName [35], "-_-.WAV");

	strcpy(Ears.cTell [36], "- - -");
	strcpy(Ears.cName [36], "-_-_-.WAV");

	strcpy(Ears.cTell [37], "Pawn");
	strcpy(Ears.cName [37], "PAWN.WAV");

	strcpy(Ears.cTell [38], "Bishop");
	strcpy(Ears.cName [38], "BISHOP.WAV");

	strcpy(Ears.cTell [39], "Knight");
	strcpy(Ears.cName [39], "KNIGHT.WAV");

	strcpy(Ears.cTell [40], "Rook");
	strcpy(Ears.cName [40], "ROOK.WAV");

	strcpy(Ears.cTell [41], "Queen");
	strcpy(Ears.cName [41], "QUEEN.WAV");

	strcpy(Ears.cTell [42], "I lose...");
	strcpy(Ears.cName [42], "ILOSE.WAV");

	strcpy(Ears.cTell [43], "Can I go?");
	strcpy(Ears.cName [43], "CANIGO.WAV");

	strcpy(Ears.cTell [44], "Take");
	strcpy(Ears.cName [44], "TAKE.WAV");

	strcpy(Ears.cTell [45], "What u want?");
	strcpy(Ears.cName [45], "WHATUWANT.WAV");

	strcpy(Ears.cTell [46], "No!");
	strcpy(Ears.cName [46], "NO.WAV");

	strcpy(Ears.cTell [47], "I Mated");
	strcpy(Ears.cName [47], "IMATED.WAV");

	strcpy(Ears.cTell [48], "Opp Mated");
	strcpy(Ears.cName [48], "OPPMATED.WAV");

	// new SuperGrover sounds
	strcpy(Ears.cTell [49], "We up");
	strcpy(Ears.cName [49], "WEUP.WAV");

	strcpy(Ears.cTell [50], "We down");
	strcpy(Ears.cName [50], "WEDOWN.WAV");

	strcpy(Ears.cTell [51], "U get");
	strcpy(Ears.cName [51], "YOUGET.WAV");

	strcpy(Ears.cTell [52], "He gets");
	strcpy(Ears.cName [52], "HEGETS.WAV");

	strcpy(Ears.cTell [53], "Watchout");
	strcpy(Ears.cName [53], "WATCHOUT.WAV");

	strcpy(Ears.cTell [54], "Feed me");
	strcpy(Ears.cName [54], "FEEDME.WAV");

	strcpy(Ears.cTell [55], "Lag");
	strcpy(Ears.cName [55], "LAG.WAV");

	strcpy(Ears.cTell [56], "Lag cheating fuckers");
	strcpy(Ears.cName [56], "LAGCHEATINGFUCKERS.WAV");

	strcpy(Ears.cTell [57], "Mother fucking lag");
	strcpy(Ears.cName [57], "MOTHERLAG.WAV");

	strcpy(Ears.cTell [58], "Bitch is dead");
	strcpy(Ears.cName [58], "BITCHDEAD.WAV");

	strcpy(Ears.cTell [59], "Bite me");
	strcpy(Ears.cName [59], "BITEME.WAV");

	strcpy(Ears.cTell [60], "Cake");
	strcpy(Ears.cName [60], "CAKE.WAV");

	strcpy(Ears.cTell [61], "Eat shit");
	strcpy(Ears.cName [61], "EATSHIT.WAV");

	strcpy(Ears.cTell [62], "Fuck u");
	strcpy(Ears.cName [62], "FUKYOUMOTHER.WAV");

	strcpy(Ears.cTell [63], "I finger focked");
	strcpy(Ears.cName [63], "IFINGERFOCKED.WAV");

	strcpy(Ears.cTell [64], "I finger focking");
	strcpy(Ears.cName [64], "IFINGERFOCKING.WAV");

	strcpy(Ears.cTell [65], "Move faster bitch");
	strcpy(Ears.cName [65], "MOVEFASTBITCH.WAV");

	strcpy(Ears.cTell [66], "Son of a bitch");
	strcpy(Ears.cName [66], "SONOFABITCH.WAV");

	strcpy(Ears.cTell [67], "So easy");
	strcpy(Ears.cName [67], "SOEASY.WAV");

	strcpy(Ears.cTell [68], "She dead");
	strcpy(Ears.cName [68], "SHEDEAD.WAV");

	strcpy(Ears.cTell [69], "They suck");
	strcpy(Ears.cName [69], "THEYSUCK.WAV");
}

void EAR_NewFICSEAR(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		Ears.bPlay [nI] = 0;
		Ears.bBeep [nI] = 0;
		strcpy(Ears.cTell [nI], "");
		strcpy(Ears.cName [nI], "");
		Ears.pData [nI] = NULL;
	}

	// initialize new ear default
	strcpy(Ears.cTell [0], "Sit");
	strcpy(Ears.cName [0], "SIT.WAV");

	strcpy(Ears.cTell [1], "Go");
	strcpy(Ears.cName [1], "GO.WAV");

	strcpy(Ears.cTell [2], "Fast");
	strcpy(Ears.cName [2], "PLAYFAST.WAV");

	strcpy(Ears.cTell [3], "Time");
	strcpy(Ears.cName [3], "WATCHTIME.WAV");

	strcpy(Ears.cTell [4], "+");
	strcpy(Ears.cName [4], "+.WAV");

	strcpy(Ears.cTell [5], "++");
	strcpy(Ears.cName [5], "++.WAV");

	strcpy(Ears.cTell [6], "+++");
	strcpy(Ears.cName [6], "+++.WAV");

	strcpy(Ears.cTell [7], "-");
	strcpy(Ears.cName [7], "-.WAV");

	strcpy(Ears.cTell [8], "- -");
	strcpy(Ears.cName [8], "-_-.WAV");

	strcpy(Ears.cTell [9], "- - -");
	strcpy(Ears.cName [9], "-_-_-.WAV");

	strcpy(Ears.cTell [10], "Pawn");
	strcpy(Ears.cName [10], "PAWN.WAV");

	strcpy(Ears.cTell [11], "Bishop");
	strcpy(Ears.cName [11], "BISHOP.WAV");

	strcpy(Ears.cTell [12], "Knight");
	strcpy(Ears.cName [12], "KNIGHT.WAV");

	strcpy(Ears.cTell [13], "Rook");
	strcpy(Ears.cName [13], "ROOK.WAV");

	strcpy(Ears.cTell [14], "Queen");
	strcpy(Ears.cName [14], "QUEEN.WAV");

	strcpy(Ears.cTell [15], "I lose...");
	strcpy(Ears.cName [15], "ILOSE.WAV");

	strcpy(Ears.cTell [16], "OK");
	strcpy(Ears.cName [16], "OK.WAV");

	strcpy(Ears.cTell [17], "OK Now");
	strcpy(Ears.cName [17], "OKNOW.WAV");

	strcpy(Ears.cTell [18], "Hard");
	strcpy(Ears.cName [18], "HARD.WAV");

	strcpy(Ears.cTell [19], "Coming");
	strcpy(Ears.cName [19], "COMING.WAV");

	strcpy(Ears.cTell [20], "Maybe");
	strcpy(Ears.cName [20], "MAYBE.WAV");

	strcpy(Ears.cTell [21], "I sit");
	strcpy(Ears.cName [21], "ISIT.WAV");

	strcpy(Ears.cTell [22], "Can I go?");
	strcpy(Ears.cName [22], "CANIGO.WAV");

	strcpy(Ears.cTell [23], "Nevermind");
	strcpy(Ears.cName [23], "NEVERMIND.WAV");

	strcpy(Ears.cTell [24], "Take");
	strcpy(Ears.cName [24], "TAKE.WAV");

	strcpy(Ears.cTell [25], "What u want?");
	strcpy(Ears.cName [25], "WHATUWANT.WAV");

	strcpy(Ears.cTell [26], "No!");
	strcpy(Ears.cName [26], "NO.WAV");

	strcpy(Ears.cTell [27], "Keep Checking");
	strcpy(Ears.cName [27], "KEEPCHECKING.WAV");

	strcpy(Ears.cTell [28], "Tell Me Go");
	strcpy(Ears.cName [28], "TELLMEGO.WAV");

	strcpy(Ears.cTell [29], "Sit Until I Tell U Go");
	strcpy(Ears.cName [29], "TELLUGO.WAV");

	strcpy(Ears.cTell [30], "I Mated");
	strcpy(Ears.cName [30], "IMATED.WAV");

	strcpy(Ears.cTell [31], "Opp Mated");
	strcpy(Ears.cName [31], "OPPMATED.WAV");

	// include old FICS ear default
	strcpy(Ears.cTell [32], "----");
	strcpy(Ears.cName [32], "NOTRADE.WAV");

	strcpy(Ears.cTell [33], "+Q");
	strcpy(Ears.cName [33], "QGOOD.WAV");

	strcpy(Ears.cTell [34], "-Q");
	strcpy(Ears.cName [34], "NOQ.WAV");

	strcpy(Ears.cTell [35], "+R");
	strcpy(Ears.cName [35], "RGOOD.WAV");

	strcpy(Ears.cTell [36], "-R");
	strcpy(Ears.cName [36], "NOR.WAV");

	strcpy(Ears.cTell [37], "+N");
	strcpy(Ears.cName [37], "NGOOD.WAV");

	strcpy(Ears.cTell [38], "-N");
	strcpy(Ears.cName [38], "NON.WAV");

	strcpy(Ears.cTell [39], "+B");
	strcpy(Ears.cName [39], "BGOOD.WAV");

	strcpy(Ears.cTell [40], "-B");
	strcpy(Ears.cName [40], "NOB.WAV");

	strcpy(Ears.cTell [41], "+P");
	strcpy(Ears.cName [41], "PGOOD.WAV");

	strcpy(Ears.cTell [42], "-P");
	strcpy(Ears.cName [42], "NOP.WAV");

	strcpy(Ears.cTell [43], "Mates me");
	strcpy(Ears.cName [43], "MATESME.WAV");

	strcpy(Ears.cTell [44], "Mates him");
	strcpy(Ears.cName [44], "MATESHIM.WAV");

	strcpy(Ears.cTell [45], "I dead");
	strcpy(Ears.cName [45], "IMDEAD.WAV");

	strcpy(Ears.cTell [46], "Opp dead");
	strcpy(Ears.cName [46], "OPPMATED.WAV");

	strcpy(Ears.cTell [47], "Yes");
	strcpy(Ears.cName [47], "YES.WAV");

	strcpy(Ears.cTell [48], "No");
	strcpy(Ears.cName [48], "NO.WAV");

	// new SuperGrover sounds
	strcpy(Ears.cTell [49], "We up");
	strcpy(Ears.cName [49], "WEUP.WAV");

	strcpy(Ears.cTell [50], "We down");
	strcpy(Ears.cName [50], "WEDOWN.WAV");

	strcpy(Ears.cTell [51], "U get");
	strcpy(Ears.cName [51], "YOUGET.WAV");

	strcpy(Ears.cTell [52], "He gets");
	strcpy(Ears.cName [52], "HEGETS.WAV");

	strcpy(Ears.cTell [53], "Watchout");
	strcpy(Ears.cName [53], "WATCHOUT.WAV");

	strcpy(Ears.cTell [54], "Feed me");
	strcpy(Ears.cName [54], "FEEDME.WAV");

	strcpy(Ears.cTell [55], "Lag");
	strcpy(Ears.cName [55], "LAG.WAV");

	strcpy(Ears.cTell [56], "Lag cheating fuckers");
	strcpy(Ears.cName [56], "LAGCHEATINGFUCKERS.WAV");

	strcpy(Ears.cTell [57], "Mother fucking lag");
	strcpy(Ears.cName [57], "MOTHERLAG.WAV");

	strcpy(Ears.cTell [58], "Bitch is dead");
	strcpy(Ears.cName [58], "BITCHDEAD.WAV");

	strcpy(Ears.cTell [59], "Bite me");
	strcpy(Ears.cName [59], "BITEME.WAV");

	strcpy(Ears.cTell [60], "Cake");
	strcpy(Ears.cName [60], "CAKE.WAV");

	strcpy(Ears.cTell [61], "Eat shit");
	strcpy(Ears.cName [61], "EATSHIT.WAV");

	strcpy(Ears.cTell [62], "Fuck u");
	strcpy(Ears.cName [62], "FUKYOUMOTHER.WAV");

	strcpy(Ears.cTell [63], "I finger focked");
	strcpy(Ears.cName [63], "IFINGERFOCKED.WAV");

	strcpy(Ears.cTell [64], "I finger focking");
	strcpy(Ears.cName [64], "IFINGERFOCKING.WAV");

	strcpy(Ears.cTell [65], "Move faster bitch");
	strcpy(Ears.cName [65], "MOVEFASTBITCH.WAV");

	strcpy(Ears.cTell [66], "Son of a bitch");
	strcpy(Ears.cName [66], "SONOFABITCH.WAV");

	strcpy(Ears.cTell [67], "So easy");
	strcpy(Ears.cName [67], "SOEASY.WAV");

	strcpy(Ears.cTell [68], "She dead");
	strcpy(Ears.cName [68], "SHEDEAD.WAV");

	strcpy(Ears.cTell [69], "They suck");
	strcpy(Ears.cName [69], "THEYSUCK.WAV");
}

void EAR_Load(void)
{
	int         nI;
	FILE        *Fv;
	struct stat stStat;

	EAR_Destroy();
	EAR_Pack();

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		if(Ears.bBeep [nI])
		{
			Ears.bPlay [nI] = 1;
		}
		else if((strlen(Ears.cTell [nI]) > 0) && (strlen(Ears.cName [nI]) > 0))
		{
			Fv = fopen(TOOLBOX_GetFullWaveFilename(Ears.cName [nI]), "rb");
			if(Fv != NULL)
			{
				if(fstat(fileno(Fv), &stStat) >= 0)
				{
					Ears.pData [nI] = malloc(stStat.st_size);

					if(fread(Ears.pData [nI], stStat.st_size, 1, Fv) >= 1)
					{
						Ears.bPlay [nI] = 1;
					}
				}
				fclose(Fv);
			}
		}
	}
}

int EAR_Play(char *cSt)
{
	int nI, nP = 0;
	char *cPtr;

	if(User.bSound && User.bEarSound)
	{
		cPtr = strstr(cSt, ": ");
		if(cPtr == NULL)
		{
			return 0;
		}

		nI = strlen(cSt);
		if(cSt [nI - 1] == '\n')
		{
			cSt [nI - 1] = NULL_CHAR;
		}

		cPtr += 2;

		for(nI = 0 ; nI < MAX_EAR ; nI++)
		{
			if(! Ears.bPlay [nI])
			{
				break;
			}

			if(stricmp(Ears.cTell [nI], cPtr) == 0)
			{
				if(Ears.bBeep [nI])
				{
					TOOLBOX_Beep();
				}
				else
				{
					PlaySound((char *) Ears.pData [nI], NULL, SND_MEMORY | SND_ASYNC);
				}
				nP = 1;
				break;
			}
		}
	}
	return nP;
}

void EAR_Pack(void)
{
	int nI, nJ;

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		if((strlen(Ears.cTell [nI]) == 0) || (strlen(Ears.cName [nI]) == 0))
		{
			for(nJ = nI + 1 ; nJ < MAX_EAR ; nJ++)
			{
				if((strlen(Ears.cTell [nJ]) > 0) &&
						(strlen(Ears.cName [nJ]) > 0))
				{
					strcpy(Ears.cTell [nI], Ears.cTell [nJ]);
					strcpy(Ears.cName [nI], Ears.cName [nJ]);
					strcpy(Ears.cTell [nJ], "");
					strcpy(Ears.cName [nJ], "");
					break;
				}
			}
		}
	}
}

void EAR_Destroy(void)
{
	int nI;

	for(nI = 0 ; nI < MAX_EAR ; nI++)
	{
		Ears.bPlay [nI] = 0;
		if(Ears.pData [nI])
		{
			free(Ears.pData [nI]);
		}
		Ears.pData [nI] = NULL;
	}
}
