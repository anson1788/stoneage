#include <stdlib.h>
#include "version.h"
#include "link.h"
#include "buf.h"

/*
 * Â¦ÐÑ
 *      top         ÐþÓÀÃóÓÃ¡õÓñ
 * ß¯Ô»°À
 */
BOOL Nodeappendtail( Node** top  , Node* add )
{
    Node* c;        /*»ï¡õÃóåÃ*/
    Node* next;     /*Þ¥ØÆÈÊ×ÛÔÂÓÃ¡õÓñåÃ*/

    /*ÐþÓÀÃó»¥NULL¾®Éýµ¤¾®¼°ÃñÄáÓÀÛÍ*/
    if( *top == NULL ){
        *top = allocateMemory( sizeof( Node  ) );
        if( *top== NULL ) return FALSE;
        (*top)->next    = NULL;            /*Á¸Îå·´  ÖÐ*/
        (*top)->size    = add->size;   /*Ó®½ñ¼°âç¶°*/
        (*top)->val     = add->val;         /*ºÌÄÌ¼þÕý¡õ¼°Îìñâ¡õ*/
        return TRUE;
    }

    for( c = *top ; c->next ; c = c->next ); /* c¼°ÞË  Ã«ÄÚÈÕÔÊ */
    next = allocateMemory( sizeof(Node) );
    if( next == NULL )return FALSE;
    c->next = next;                         /* next±å°ÀÃ«É¬ÀÃÔÊÔÂ */
    next->next  = NULL;                      /*Á¸Îå·´  ÖÐ*/
    next->val   = add->val;                   /*ºÌÄÌ¼þÕý¡õ¼°Îìñâ¡õ*/
    next->size  = add->size;             /*Ó®½ñ¼°âç¶°*/
    return TRUE;
}




/*
 *  Øø¼þÛÍ¼°ó¡±å¹´ØêÔÂ
 * Â¦ÐÑ
 *      top         ÐþÓÀÃóÓÃ¡õÓñ
 * ß¯Ô»°À
 */
BOOL Nodeappendhead( Node** nowtop  , Node* add )
{
    Node* newtop;       /*Þ¥ØÆÖÐÛÆ  åÃ*/

    /*ÐþÓÀÃó»¥NULL¾®Éýµ¤¾®¼°ÃñÄáÓÀÛÍ*/
    if( *nowtop == NULL ){
        *nowtop = allocateMemory( sizeof( Node  ) );
        if( *nowtop == NULL ) return FALSE;
        (*nowtop)->next = NULL;             /*Á¸Îå·´  ÖÐ*/
        (*nowtop)->size = add->size;        /*Ó®½ñ¼°âç¶°*/
        (*nowtop)->val  = add->val;         /*ºÌÄÌ¼þÕý¡õ¼°Îìñâ¡õ*/
        return TRUE;
    }

    /*
     * Þ¥ØÆÖÐÓÃ¡õÓñÃ«Óò¹´×ÛÔÂ
     */
    newtop = allocateMemory( sizeof(Node) );
    newtop->next    = *nowtop;
    newtop->val     = add->val;
    newtop->size    = add->size;
    *nowtop = newtop;
    return TRUE;
}




/*
 * Â¦ÐÑ
 *          ret         äúÔ»ÂÖÖÐÐ×ÓÃ¡õÓñ¼°Îìñâ¡õ
 * ß¯Ô»°À
 */
BOOL  Noderemovehead( Node** top , Node* ret)
{
    Node* newtop;         /*Þ¥ØÆÈÊÛÆ  ±åØ¦ÔÂÓÃ¡õÓñ*/

    if( *top == NULL )return FALSE;

    ret->val    = (*top)->val;
    ret->size   = (*top)->size;
    newtop = (*top)->next;
    freeMemory( *top );
    *top = newtop;

    return TRUE;
}





/*
 * Â¦ÐÑ
 *          ret         äúÔ»ÂÖÖÐÐ×ÓÃ¡õÓñ¼°Îìñâ¡õ
 * ß¯Ô»°À
 */
BOOL  Noderemovetail( Node** top , Node* ret)
{
    Node* c;             /*»ï¡õÃóåÃ*/
    Node* c1;            /*»ï¡õÃóåÃ ÖÐ¹´Æ¥ÊÖ c->nextÃ«Ï¶ÔÊ*/

    if( *top == NULL )return FALSE;

    c = *top;           /*âÙÓå°ÀÉ¬ÀÃ*/
    c1 = c->next;       /*âÙÓå°ÀÉ¬ÀÃ*/
    while(1){
        if( c1->next == NULL )
            /*
             * ³ð¼°ÁÝ÷§Æ¥          c1
             *                      |
             *         | next |---->+------+
             *         |------|     | next |---->NULL
             *         |      |     +------+
             *         +------+     |      |
             *                      +------+
             *ÎçØ¦ÔÈ»¯ÖÐÔÂ
             */
            break;
        c=c->next;
        c1=c->next;
    }
    c->next = NULL;     /*c1±åØ¤Ð×ÔÂÊÖ¼°Ã«´ÂÂÖÔÊÔÂ¼°Æ¥next·´NULLÎçÔÊÔÂ*/
    /*Îìñâ¡õ*/
    ret->val    = c1->val;
    ret->size   = c1->size;

    freeMemory( c1 );

    return TRUE;
}
