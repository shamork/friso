/*
 * main interface file for friso - free soul.
 * 		you could modify it and re-release it but never for commercial use.
 * 
 * @author	chenxin
 * @email	chenxin619315@gmail.com 
 */
#ifndef _friso_h
#define _friso_h

#include "friso_API.h"
#include <stdio.h>

/* {{{ friso main interface define :: start*/
#define DEFAULT_SEGMENT_LENGTH 	5
#define DEFAULT_MIX_LENGTH	2
#define DEFAULT_LNA_LENGTH 	1
#define DEFAULT_NTHRESHOLD 	1000000
#define DEFAULT_SEGMENT_MODE 	2

/*
 * Type: friso_lex_t
 * -----------
 * This type used to represent the type of the lexicon. 
 */
typedef enum {
    __LEX_CJK_WORDS__ = 0,
    __LEX_CJK_UNITS__ = 1,
    __LEX_ECM_WORDS__ = 2,	//english and chinese mixed words.
    __LEX_CEM_WORDS__ = 3,	//chinese and english mixed words.
    __LEX_CN_LNAME__ = 4,
    __LEX_CN_SNAME__ = 5,
    __LEX_CN_DNAME1__ = 6,
    __LEX_CN_DNAME2__ = 7,
    __LEX_CN_LNA__ = 8,
    __LEX_STOPWORDS__ = 9,
    __LEX_ENPUN_WORDS__ = 10,
    __LEX_EN_WORDS__ = 11,
    __LEX_OTHER_WORDS__ = 15,
    __LEX_NCSYN_WORDS__ = 16
} friso_lex_t;

typedef friso_hash_t * friso_dic_t;
#define __FRISO_LEXICON_LENGTH__ 12 

/*
 * Type: lex_entry_cdt
 * -------------------
 * This type used to represent the lexicon entry struct. 
 */
typedef struct {
    /*
     * the type of the lexicon item.
     * available value is all the elements in friso_lex_t enum.
     *	and if it is __LEX_OTHER_WORDS__, we need to free it after use it.
     *	here use char as its data type not int.
     */
    uint_t length;
    uint_t type;					
    fstring word;
    friso_array_t syn;
    uint_t fre;
} lex_entry_cdt;
typedef lex_entry_cdt * lex_entry_t;



/*
 * Type: friso_mode_t
 * ------------------
 * use to identidy the mode that the friso use. 
 */
typedef enum {
    __FRISO_SIMPLE_MODE__ 	= 1,
    __FRISO_COMPLEX_MODE__ 	= 2
} friso_mode_t;

/*
 * Type: friso_entry
 * -----------------
 * This type is used to set the configuration of friso. 
 */
typedef struct {
    ushort_t max_len;		//the max match length (4 - 7).
    ushort_t r_name;		//1 for open chinese name recognition 0 for close it.
    ushort_t mix_len;		//the max length for the CJK words in a mix string.
    ushort_t lna_len;		//the max length for the chinese last name adron.
    ushort_t add_syn;		//append synonyms tokenizer words.
    ushort_t clr_stw;		//clear the stopwords.
    ushort_t keep_urec;		//keep the unrecongnized words.
    ushort_t spx_out;		//use sphinx output customize.
    uint_t nthreshold;		//the threshold value for a char to make up a chinese name.
    friso_mode_t mode;		//Complex mode or simple mode
    friso_dic_t dic;		//friso dictionary
} friso_entry;

typedef friso_entry * friso_t;


#define __HITS_WORD_LENGTH__ 128
/*the segmentation term*/
typedef struct {
    int offset;
    char word[__HITS_WORD_LENGTH__];
} friso_hits_entry;

typedef friso_hits_entry * friso_hits_t;

/*
 * Type: friso_segment
 * This type used to represent the current segmentation content.
 * 		like the text to split, and the current index. 
 */
typedef struct {
    fstring text;		//text to tokenize
    uint_t idx;			//start offset index.
    uint_t length;		//length of the text.
    uint_t bytes;		//latest word bytes in C.
    uint_t unicode;		//latest word unicode number.
    //uint_t ce_check;		//check the CN and EN mixed word if it is 1.
    friso_link_t pool;		//task pool.
    friso_hits_t hits;		//token result hits.
    char buffer[7];		//word buffer. (1-6 bytes for an utf-8 word in C).
} friso_task_entry;

typedef friso_task_entry * friso_task_t;


/*
 * Function: friso_new;
 * Usage: vars = friso_new( void );
 * --------------------------------
 * This function used to create a new empty friso friso_t; 
 *		with default value.
 */
FRISO_API friso_t friso_new( void );

//creat a friso entry with a default value from a configuratile file.
FRISO_API friso_t friso_new_from_ifile( fstring );

/*
 * Function: friso_free_vars;
 * Usage: friso_free( vars );
 * --------------------------
 * This function is used to free the allocation of the given vars. 
 */
FRISO_API void friso_free( friso_t );

/*
 * Function: friso_set_dic
 * Usage: dic = friso_set_dic( vars, dic );
 * ----------------------------------------
 * This function is used to set the dictionary for friso. 
 * 		and firso_dic_t is the pointer of a hash table array.
 */
//FRISO_API void friso_set_dic( friso_t, friso_dic_t );
#define friso_set_dic(friso, dic)\
    friso->dic = dic

/*
 * Function: friso_set_mode
 * Usage: friso_set_mode( vars, mode );
 * ------------------------------------
 * This function is used to set the mode(complex or simple) that you want to friso to use.
 */
//FRISO_API void friso_set_mode( friso_t, friso_mode_t );
#define friso_set_mode( friso, mode )\
    friso->mode = mode

/*
 * Function: friso_new_task;
 * Usage: segment = friso_new_task( void );
 * ----------------------------------------
 * This function is used to create a new friso segment type; 
 */
FRISO_API friso_task_t friso_new_task( void );

/*
 * Function: friso_free_task;
 * Usage: friso_free_task( task ); 
 * -------------------------------
 * This function is used to free the allocation of function friso_new_segment();
 */
FRISO_API void friso_free_task( friso_task_t );

//create a new friso hits
FRISO_API friso_hits_t friso_new_hits( void );

//free the given friso hits
//FRISO_API void friso_free_hits( friso_hits_t );
#define friso_free_hits(hits) FRISO_FREE(hits)

/*
 * Function: friso_set_text
 * Usage: friso_set_text( task, text );
 * ------------------------------------
 * This function is used to set the text that is going to segment. 
 */
FRISO_API void friso_set_text( friso_task_t, fstring );

/*
 * Function: friso_next
 * Usage: word = friso_next( vars, seg );
 * --------------------------------------
 * This function is used to get next word that friso segmented. 
 */
FRISO_API friso_hits_t friso_next( friso_t, friso_mode_t, friso_task_t );
/* }}} friso main interface define :: end*/

/* {{{ lexicon interface define :: start*/

/*
 * Function: friso_dic_new
 * Usage: dic = friso_new_dic();
 * -----------------------------
 * This function used to create a new dictionary.(memory allocation).
 */
FRISO_API friso_dic_t friso_dic_new( void );

FRISO_API fstring file_get_line( fstring, FILE * );

/*
 * Function: friso_dic_free
 * Usage: friso_dic_free( void );
 * ------------------------------
 * This function is used to free all the allocation of friso_dic_new. 
 */
FRISO_API void friso_dic_free( friso_dic_t );

//create a new lexicon entry.
FRISO_API lex_entry_t new_lex_entry( fstring, friso_array_t, uint_t, uint_t, uint_t );

//free the given lexicon entry.
//free all the allocations that its synonyms word's items pointed to
//when the second arguments is 1
FRISO_API void free_lex_entry( lex_entry_t );

/*
 * Function: friso_dic_load
 * Usage: friso_dic_load( friso, friso_lex_t, path, length ); 
 * --------------------------------------------------
 * This function is used to load dictionary from a given path.
 * 		no length limit when length less than 0.
 */
FRISO_API void friso_dic_load( friso_t, friso_lex_t, fstring, uint_t );

/*
 * load the lexicon configuration file.
 *	and load all the valid lexicon from the conf file.
 */
FRISO_API void friso_dic_load_from_ifile( friso_t, fstring, uint_t );

/*
 * Function: friso_dic_match
 * Usage: friso_dic_add( dic, friso_lex_t, word, syn );
 * ----------------------------------------------
 * This function used to put new word into the dictionary.
 */
FRISO_API void friso_dic_add( friso_dic_t, friso_lex_t, fstring, friso_array_t );

/*
 * Function: friso_dic_add_with_fre
 * Usage: friso_dic_add_with_fre( dic, friso_lex_t, word, value, syn, fre );
 * -------------------------------------------------------------------
 * This function used to put new word width frequency into the dictionary.
 */
FRISO_API void friso_dic_add_with_fre( friso_dic_t, friso_lex_t, fstring, friso_array_t, uint_t );

/*
 * Function: friso_dic_match
 * Usage: result = friso_dic_match( dic, friso_lex_t, word );
 * ----------------------------------------------------
 * This function is used to check the given word is in the dictionary or not. 
 */
FRISO_API int friso_dic_match( friso_dic_t, friso_lex_t, fstring );

/*
 * Function: friso_dic_get
 * Usage: friso_dic_get( dic, friso_lex_t, word );
 * -----------------------------------------
 * This function is used to search the specified lex_entry_t.
 */
FRISO_API lex_entry_t friso_dic_get( friso_dic_t, friso_lex_t, fstring );

/*
 * Function: friso_spec_dic_size
 * Usage: friso_spec_dic_size( dic, friso_lex_t )
 * This function is used to get the size of the dictionary with a specified type. 
 */
FRISO_API uint_t friso_spec_dic_size( friso_dic_t, friso_lex_t );
FRISO_API uint_t friso_all_dic_size( friso_dic_t );
/* }}} lexicon interface define :: end*/


#endif /*end ifndef*/