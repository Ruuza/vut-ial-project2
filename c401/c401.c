/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**          Implementace: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>, říjen 2017
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"


int solved;


void BSTInit(tBSTNodePtr *RootPtr)
{
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	*RootPtr = NULL; // nastavení hodnoty ukazatele na kořen BVS na hodnotu NULL
}


int BSTSearch(tBSTNodePtr RootPtr, char K, int *Content)
{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu. Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
	// Pokud bude ukazatel na kořen NULL, tak vrátíme FALSE. (Ukončení rekurze v případě, že položka nebyla nalezena.)
	if (!RootPtr)
	{
		return FALSE;
	}

	// Uzel s klíčem K byl nalezen. (Ukončení rekurze v případě, že byla položka nalezena.)
	if (RootPtr->Key == K)
	{
		// Vrácení obsahu nalezeného uzlu v proměnné Content a vrácení TRUE.
		*Content = RootPtr->BSTNodeCont;
		return TRUE;
	}

	// Hledaný klíč je menší než klíč kořene, proto zavoláme funkci rekurzívně pro levý podstrom.
	if (K < RootPtr->Key)
	{
		return BSTSearch(RootPtr->LPtr, K, Content);
	}

	// Hledaný klíč je větší než klíč kořene, proto zavoláme funkci rekurzívně pro pravý podstrom.
	return BSTSearch(RootPtr->RPtr, K, Content);
}


void BSTInsert(tBSTNodePtr *RootPtr, char K, int Content)
{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
	if (!*RootPtr) // strom je prázdný (ukončení rekurze)
	{
		// vložená položka bude kořenem stromu
		if (!((*RootPtr) = (tBSTNodePtr) malloc(sizeof(struct tBSTNode))))
		{
			return; // malloc selhal
		}
		// nastavení parametrů vložené položky
		(*RootPtr)->Key = K;
		(*RootPtr)->BSTNodeCont = Content;
		(*RootPtr)->LPtr = (*RootPtr)->RPtr = NULL;

		return;
	}

	if ((*RootPtr)->Key == K) // uzel se zadaným klíčem již ve stromu existuje
	{
		// nahrazení hodnoty uzlu s existujícím klíčem
		(*RootPtr)->BSTNodeCont = Content;
		return;
	}

	if (K < (*RootPtr)->Key) // zadaný klíč je menší než klíč kořene
	{
		// Zavoláme funkci rekurzívně pro levý podstrom.
		BSTInsert(&((*RootPtr)->LPtr), K, Content);
		return;
	}

	// zadaný klíč je větší než klíč kořene
	// Zavoláme funkci rekurzívně pro pravý podstrom.
	BSTInsert(&((*RootPtr)->RPtr), K, Content);
}


void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr)
{
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/
	tBSTNodePtr right_most;

	if (!(*RootPtr)->RPtr) // kořen nemá žádný pravý podstrom (může nastat pouze v případě vnějšího volání funkce)
	{
		right_most = *RootPtr; // Nejpravější uzel je sám kořen, ten bude smazán.

		// přesun hodnot z nejpravějšího uzlu
		PtrReplaced->Key = right_most->Key;
		PtrReplaced->BSTNodeCont = right_most->BSTNodeCont;

		// Pokud nejpravější uzel měl levý podstrom, je potřeba nastavit tento jako levý podstrom
		// uzlu, do kterého se přesouvá jeho hodnota, jinak je potřeba nastavit tento ukazatel na NULL.
		PtrReplaced->LPtr = NULL;
		if (right_most->LPtr)
		{
			PtrReplaced->LPtr = right_most->LPtr;
		}

		free(right_most); // uvolnění nejpravějšího uzlu
		return;
	}

	if (!(*RootPtr)->RPtr->RPtr) // pravý podstrom nemá žádný další pravý podstrom (ukončení rekurze)
	{
		right_most = (*RootPtr)->RPtr; // nejpravější uzel je pravý podstrom kořenového uzlu

		// přesun hodnot z nejpravějšího uzlu
		PtrReplaced->Key = right_most->Key;
		PtrReplaced->BSTNodeCont = right_most->BSTNodeCont;

		// Pokud nejpravější uzel měl levý podstrom, je potřeba nastavit tento jako pravý podstrom kořenového
		// uzlu, jinak je potřeba nastavit tento ukazatel na NULL.
		(*RootPtr)->RPtr = NULL;
		if (right_most->LPtr)
		{
			(*RootPtr)->RPtr = right_most->LPtr;
		}

		free(right_most); // uvolnění nejpravějšího uzlu
		return;
	}

	// zavolání funkce rekurzívně pro pravý podstrom
	ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));
}


void BSTDelete(tBSTNodePtr *RootPtr, char K)
{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	if (!*RootPtr) // strom je prázdný (ukončení rekurze)
	{
		return;
	}

	tBSTNodePtr node_to_delete;

	////////// hledaný klíč je menší než klíč kořene
	if (K < (*RootPtr)->Key)
	{
		if ((*RootPtr)->LPtr && (*RootPtr)->LPtr->Key == K) // klíč kořene levého podstromu je hledaný klíč
		{
			node_to_delete = (*RootPtr)->LPtr;

			if (node_to_delete->LPtr && node_to_delete->RPtr) // uzel má dva podstromy
			{
				// nahrazení uzlu nejpravějším uzlem levého podstromu
				ReplaceByRightmost(node_to_delete, &(node_to_delete->LPtr));
				return;
			}

			if (node_to_delete->LPtr && !node_to_delete->RPtr) // uzel má pouze levý podstrom
			{
				(*RootPtr)->LPtr = node_to_delete->LPtr; // korekce ukazatele v otci
			}
			else if (!node_to_delete->LPtr && node_to_delete->RPtr) // uzel má pouze pravý podstrom
			{
				(*RootPtr)->LPtr = node_to_delete->RPtr; // korekce ukazatele v otci
			}
			else // uzel nemá žádný podstrom
			{
				(*RootPtr)->LPtr = NULL; // korekce ukazatele v otci
			}

			free(node_to_delete); // uvolnění mazaného uzlu z paměti
			return;
		}

		// zavolání funkce rekurzívně pro levý podstrom
		BSTDelete(&((*RootPtr)->LPtr), K);
		return;
	}

	////////// hledaný klíč je větší než klíč kořene
	if (K > (*RootPtr)->Key)
	{
		if ((*RootPtr)->RPtr && (*RootPtr)->RPtr->Key == K) // klíč kořene pravého podstromu je hledaný klíč
		{
			node_to_delete = (*RootPtr)->RPtr;

			if (node_to_delete->LPtr && node_to_delete->RPtr) // uzel má dva podstromy
			{
				// nahrazení uzlu nejpravějším uzlem levého podstromu
				ReplaceByRightmost(node_to_delete, &(node_to_delete->LPtr));
				return;
			}

			if (node_to_delete->LPtr && !node_to_delete->RPtr) // uzel má pouze levý podstrom
			{
				(*RootPtr)->RPtr = node_to_delete->LPtr; // korekce ukazatele v otci
			}
			else if (!node_to_delete->LPtr && node_to_delete->RPtr) // uzel má pouze pravý podstrom
			{
				(*RootPtr)->RPtr = node_to_delete->RPtr; // korekce ukazatele v otci
			}
			else // uzel nemá žádný podstrom
			{
				(*RootPtr)->RPtr = NULL; // korekce ukazatele v otci
			}

			free(node_to_delete); // uvolnění mazaného uzlu z paměti
			return;
		}

		// zavolání funkce rekurzívně pro pravý podstrom
		BSTDelete(&((*RootPtr)->RPtr), K);
		return;
	}

	////////// hledaný klíč je klíč kořene
	node_to_delete = *RootPtr;

	if (node_to_delete->LPtr && node_to_delete->RPtr) // uzel má dva podstromy
	{
		// nahrazení uzlu nejpravějším uzlem levého podstromu
		ReplaceByRightmost(node_to_delete, &(node_to_delete->LPtr));
		return;
	}

	if (node_to_delete->LPtr && !node_to_delete->RPtr) // uzel má pouze levý podstrom
	{
		*RootPtr = node_to_delete->LPtr; // kořenový uzel bude levý podstrom
	}
	else if (!node_to_delete->LPtr && node_to_delete->RPtr) // uzel má pouze pravý podstrom
	{
		*RootPtr = node_to_delete->RPtr; // kořenový uzel bude pravý podstrom
	}
	else // uzel nemá žádný podstrom
	{
		*RootPtr = NULL; // kořenový uzel bude NULL
	}

	free(node_to_delete); // uvolnění mazaného uzlu z paměti
}


void BSTDispose(tBSTNodePtr *RootPtr)
{
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	// Ukazatel na kořen je NULL. (podmínka ukončení rekurze)
	if (!*RootPtr)
	{
		return;
	}

	// Zavoláme funkci rekurzívně pro levý i pravý podstrom.
	BSTDispose(&((*RootPtr)->LPtr));
	BSTDispose(&((*RootPtr)->RPtr));

	// Uvolnění ukazatele na kořen z paměti a nastavení na hodnotu NULL.
	free(*RootPtr);
	*RootPtr = NULL;
}
