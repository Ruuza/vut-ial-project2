/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**                                  Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**         Implementace: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>, říjen 2017
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte.
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"


int solved;


void BTWorkOut(tBTNodePtr Ptr)
{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
	if (Ptr == NULL)
	{
		printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
	}
	else
	{
		printf("Výpis hodnoty daného uzlu> %d\n", Ptr->Cont);
	}
}


/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP(tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
}


void SPushP(tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{
	/* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top == MAXSTACK)
	{
		printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
	}
	else
	{
		S->top++;
		S->a[S->top] = ptr;
	}
}


tBTNodePtr STopPopP(tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
	/* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top == 0)
	{
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return NULL;
	}
	else
	{
		return S->a[S->top--];
	}
}


bool SEmptyP(tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
	return S->top == 0;
}


/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB(tStackB *S)
{
/*   ------
** Inicializace zásobníku.
**/
	S->top = 0;
}


void SPushB(tStackB *S, bool val)
{
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
	/* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top == MAXSTACK)
	{
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	}
	else
	{
		S->top++;
		S->a[S->top] = val;
	}
}


bool STopPopB(tStackB *S)
{
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
	/* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top == 0)
	{
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return NULL;
	}
	else
	{
		return S->a[S->top--];
	}
}


bool SEmptyB(tStackB *S)
{
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
	return S->top == 0;
}


/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit(tBTNodePtr *RootPtr)
{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/
	*RootPtr = NULL; // nastavení hodnoty ukazatele na kořen BVS na hodnotu NULL
}


void BTInsert(tBTNodePtr *RootPtr, int Content)
{
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/
	// Inicializace ukazatele na uzel, za který se bude vkládat nový uzel a dalších pomocných proměnných.
	tBTNodePtr insert_after = NULL, node_iterator = *RootPtr;

	// Vyhledávání uzlu, za který budeme vkládat nový uzel.
	while (node_iterator)
	{
		insert_after = node_iterator;

		if (Content < node_iterator->Cont) // vyhledávání v levém podstromu
		{
			node_iterator = node_iterator->LPtr;
			continue;
		}
		if (Content > node_iterator->Cont) // vyhledávání v pravém podstromu
		{
			node_iterator = node_iterator->RPtr;
			continue;
		}

		return; // vkládaný uzel již existuje
	}

	// vytvoření nového uzlu stromu
	tBTNodePtr new_node = (tBTNodePtr) malloc(sizeof(struct tBTNode));
	if (!new_node)
	{
		return; // malloc selhal
	}
	// nastavení parametrů nového uzlu
	new_node->Cont = Content;
	new_node->LPtr = new_node->RPtr = NULL;

	if (!insert_after) // vkládaný uzel bude kořen
	{
		*RootPtr = new_node;
		return;
	}
	if (Content < insert_after->Cont) // vkládaný uzel bude levý list nalezeného uzlu
	{
		insert_after->LPtr = new_node;
		return;
	}
	insert_after->RPtr = new_node; // vkládaný uzel bude pravý list nalezeného uzlu
}


/*                                  PREORDER                                  */

void Leftmost_Preorder(tBTNodePtr ptr, tStackP *Stack)
{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/
	while (ptr) // Dokud ptr není NULL, opakuj:
	{
		SPushP(Stack, ptr); // uložení zpracovávaného uzlu na zásobník
		BTWorkOut(ptr); // zpracování uzlu
		ptr = ptr->LPtr; // posun na levý podstrom
	}
}


void BTPreorder(tBTNodePtr RootPtr)
{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	// inicializace zásobníku
	tStackP stack;
	SInitP(&stack);

	// Zpracování kořene funkcí Leftmost_Preorder, která navštívené uzly zpracovává a ukládá na zásobník.
	Leftmost_Preorder(RootPtr, &stack);
	while (!SEmptyP(&stack)) // Dokud zásobník není prázdný, opakuj:
	{
		// vytáhneme uzel ze zásobníku a zpracujeme jeho pravý podstrom funkcí Leftmost_Preorder
		RootPtr = STopPopP(&stack);
		Leftmost_Preorder(RootPtr->RPtr, &stack);
	}
}


/*                                  INORDER                                   */

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)
{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku. 
**/
	while (ptr) // Dokud ptr není NULL, opakuj:
	{
		SPushP(Stack, ptr); // uložení zpracovávaného uzlu na zásobník
		ptr = ptr->LPtr; // posun na levý podstrom
	}
}


void BTInorder(tBTNodePtr RootPtr)
{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	// inicializace zásobníku
	tStackP stack;
	SInitP(&stack);

	// Zpracování kořene funkcí Leftmost_Inorder, která navštívené uzly ukládá na zásobník.
	Leftmost_Inorder(RootPtr, &stack);
	while (!SEmptyP(&stack)) // Dokud zásobník není prázdný, opakuj:
	{
		// vytáhneme uzel ze zásobníku, zpracujeme ho a zpracujeme jeho pravý podstrom funkcí Leftmost_Inorder
		RootPtr = STopPopP(&stack);
		BTWorkOut(RootPtr); // zpracování uzlu
		Leftmost_Inorder(RootPtr->RPtr, &stack);
	}
}


/*                                 POSTORDER                                  */

void Leftmost_Postorder(tBTNodePtr ptr, tStackP *StackP, tStackB *StackB)
{
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat. 
**/
	while (ptr) // Dokud ptr není NULL, opakuj:
	{
		SPushP(StackP, ptr); // uložení zpracovávaného uzlu na zásobník
		SPushB(StackB, true); // uložení hodnoty true na zásobník (uzel byl navštíven poprvé)
		ptr = ptr->LPtr; // posun na levý podstrom
	}
}


void BTPostorder(tBTNodePtr RootPtr)
{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
	// inicializace zásobníků
	tStackP stack_p;
	SInitP(&stack_p);
	tStackB stack_b;
	SInitB(&stack_b);

	// Zpracování kořene funkcí Leftmost_Postorder, která navštívené uzly ukládá na zásobník s hodnotou true.
	Leftmost_Postorder(RootPtr, &stack_p, &stack_b);
	while (!SEmptyP(&stack_p)) // Dokud zásobník není prázdný, opakuj:
	{
		RootPtr = STopPopP(&stack_p); // vytáhneme uzel ze zásobníku

		if (STopPopB(&stack_b)) // uzel byl navšíven pouze jednou
		{
			// Uložíme uzel znovu na zásobník s hodnotou false (byl navštíven podruhé) a zpracujeme jeho pravý podstrom.
			SPushP(&stack_p, RootPtr);
			SPushB(&stack_b, false);
			Leftmost_Postorder(RootPtr->RPtr, &stack_p, &stack_b);

			continue;
		}

		BTWorkOut(RootPtr);  // Uzel už je navšívený podruhé, už se vracíme, proto ho zpracujeme.
	}
}


void BTDisposeTree(tBTNodePtr *RootPtr)
{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/
	// inicializace zásobníku
	tStackP stack;
	SInitP(&stack);
	tBTNodePtr node_to_delete; // Inicializace proměnné pro ukazatel na uzel, který se bude rušit.

	// Dokud ukazatel na kořen není NULL nebo není prázdný zásobník, tak opakuj:
	while (*RootPtr || !SEmptyP(&stack))
	{
		// Pokud je ukazatel na kořen NULL, tak nový kořen bude uzel ze zásobníku, kde musít být nějaký pravý podstrom.
		if (!*RootPtr)
		{
			*RootPtr = STopPopP(&stack);
		}

		// Pokud má kořen pravý podstrom, tak ho uložím na zásobník.
		if ((*RootPtr)->RPtr)
		{
			SPushP(&stack, (*RootPtr)->RPtr);
		}

		// Smažu kořen a nový kořen bude levý podstrom.
		node_to_delete = *RootPtr;
		*RootPtr = (*RootPtr)->LPtr;
		free(node_to_delete);
	}
}
