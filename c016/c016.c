/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**                      Implementace: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>, říjen 2017
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode. Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"


int HTSIZE = MAX_HTSIZE;
int solved;


/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1. V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce. V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce. (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku).
*/
int hashCode(tKey key)
{
	int retval = 1;
	int keylen = (int) strlen(key);

	for (int i = 0; i < keylen; i++)
	{
		retval += key[i];
	}

	return retval % HTSIZE;
}


/*
** Inicializace tabulky s explicitně zřetězenými synonymy. Tato procedura
** se volá pouze před prvním použitím tabulky.
*/
void htInit(tHTable *ptrht)
{
	// Nastavení všech ukazatelů na položky na hodnotu NULL.
	for (int i = 0; i < HTSIZE; i++)
	{
		(*ptrht)[i] = NULL;
	}
}


/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key. Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
*/
tHTItem *htSearch(tHTable *ptrht, tKey key)
{
	tHTItem *item = (*ptrht)[hashCode(key)]; // přisoupení k položce tabulky na indexu daném klíčem

	// Průchod přes synonyma daného klíče, dokud nebude nalezen hledaný klíč.
	for (; item; item = item->ptrnext)
	{
		// porovnání klíčů
		if (strcmp(key, item->key) == 0)
		{
			return item; // vrácení ukazatele na nalezený prvek
		}
	}

	return NULL; // prvek nebyl nalezen
}


/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data. Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát. Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch. Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte vložení prvku na začátek seznamu.
**/
void htInsert(tHTable *ptrht, tKey key, tData data)
{
	tHTItem *item = htSearch(ptrht, key); // hledání položky s daným klíčem
	// Pokud v tabulce prvek se stejným klíčem už je, aktualizuje se pouze jeho datová část.
	if (item)
	{
		item->data = data;
		return;
	}

	// vytvoření nové položky tabulky s daným klíčem a daty
	tHTItem *new_item = (tHTItem *) malloc(sizeof(tHTItem));
	if (!new_item)
	{
		return; // malloc selhal
	}
	new_item->key = key;
	new_item->data = data;
	new_item->ptrnext = NULL;

	int hash = hashCode(key); // získáni hash daného klíče
	// Pokud k danému klíči existují synonyma, tak je připojíme za nový prvek.
	if ((item = (*ptrht)[hash]))
	{
		new_item->ptrnext = item;
	}

	(*ptrht)[hash] = new_item; // uložení nového prvku na začátek seznamu daného klíče
}


/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/
tData *htRead(tHTable *ptrht, tKey key)
{
	tHTItem *item = htSearch(ptrht, key); // hledání položky s daným klíčem

	// Pokud položka s daným klíčem existuje, vátí se ukazatel na její data, jinak se vratí NULL.
	return item ? &(item->data) : NULL;
}


/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht. Uvolněnou položku korektně zrušte. Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/
void htDelete(tHTable *ptrht, tKey key)
{
	int hash = hashCode(key); // získáni hash daného klíče
	tHTItem *item = (*ptrht)[hash]; // přisoupení k položce tabulky na daném indexu
	tHTItem *prev_item = NULL, *next_item = NULL;

	// Průchod přes synonyma daného klíče, dokud nebude nalezen hledaný klíč.
	for (; item; prev_item = item, item = item->ptrnext)
	{
		next_item = item->ptrnext; // uložení ukazatele na následující položku

		// porovnání klíčů
		if (strcmp(key, item->key) == 0)
		{
			free(item); // uvolnění položky z paměti

			// hledaný prvek je na začátku seznamu
			if (!prev_item)
			{
				(*ptrht)[hash] = next_item; // nastavení následujícího prvku přímo do tabulky na index daný klíčem
				return;
			}

			// hledaný prvek je uprostřed nebo na konci seznamu
			prev_item->ptrnext = next_item; // ukazatel na další prvek předchozího prvku bude následující prvek
			return;
		}
	}
}


/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/
void htClearAll(tHTable *ptrht)
{
	tHTItem *item, *item_to_delete;

	// průchod přes všechny indexy tabulky
	for (int i = 0; i < HTSIZE; i++)
	{
		// průchod přes všechny položky zřetězených seznamů
		item = (*ptrht)[i];
		while (item)
		{
			item_to_delete = item;
			item = item->ptrnext;
			free(item_to_delete); // uvolnění položky z paměti
		}
		(*ptrht)[i] = NULL; // nastavení ukazatele na daném indexu na hodnotu NULL
	}
}
