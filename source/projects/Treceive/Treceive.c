// see https://github.com/Cycling74/max-sdk/blob/main/source/basics/simplemax/simplemax.c
// Jeremy Bernstein

#include "ext.h"         // standard Max include, always required
#include "ext_obex.h"    // required for new style Max object
#include "ext_dictobj.h"
#include "../include/testInclude.h"

////////////////////////// object struct
typedef struct _Treceive {
	t_object ob;    // the object itself (must be first)
    void *outlet;
    t_symbol *name;
    t_symbol *dictionary_id;
    t_atom *input_message;
} t_Treceive;

///////////////////////// function prototypes
//// standard set
void* Treceive_new(t_symbol* s, long argc, t_atom* argv);
void  Treceive_free(t_Treceive* x);
void  Treceive_assist(t_Treceive* x, void* b, long m, long a, char* s);
void  Treceive_anything(t_Treceive *x, t_symbol *s, long argc, t_atom *argv);
t_symbol* Treceive_getName(t_Treceive *x);

//////////////////////// global class pointer variable
void* Treceive_class;
static t_dictionary *global_receive_dictionary = NULL;
static int global_id = 0;

void ext_main(void* r) {
	t_class* c;

	c = class_new("Treceive", (method)Treceive_new, (method)Treceive_free, (long)sizeof(t_Treceive), 0L /* leave NULL!! */, A_GIMME, 0);

	/* you CAN'T call this from the patcher */
	class_addmethod(c, (method)Treceive_assist,     "assist",   A_CANT,  0);
    class_addmethod(c, (method)Treceive_anything,   "anything", A_GIMME, 0);
    class_addmethod(c, (method)Treceive_getName,    "getname",           0);

	class_register(CLASS_BOX, c); /* CLASS_NOBOX */
	Treceive_class = c;
    
    post("Treceive : %s %s", __TIME__, __DATE__);
    
    t_symbol *ps_receive_dict = gensym("receive_dictionary");
    global_receive_dictionary = dictobj_findregistered_retain(ps_receive_dict);
    if(global_receive_dictionary == NULL){
        global_receive_dictionary = dictionary_new();
        dictobj_register(global_receive_dictionary, &ps_receive_dict);
    }
}

void Treceive_assist(t_Treceive* x, void* b, long m, long a, char* s) {
	if (m == ASSIST_INLET) {    // inlet
		sprintf(s, "I am inlet %ld", a);
	}
	else {    // outlet
		sprintf(s, "I am outlet %ld", a);
	}
}

void  Treceive_anything(t_Treceive *x, t_symbol *s, long argc, t_atom *argv){
//    x->input_message = argv;
    
    switch(atom_gettype(argv)){
        case A_SYM:
            outlet_anything(x->outlet, gensym("symbol"), 1, argv);
            break;
        case A_LONG:
            outlet_anything(x->outlet, gensym("int"), 1, argv);
            break;
        case A_FLOAT:
            outlet_anything(x->outlet, gensym("float"), 1, argv);
            break;
        default:
            post("Error !");
    }
    
//    outlet_anything(x->outlet, gensym(""), 1, x->input_message);
}

t_symbol* Treceive_getName(t_Treceive *x){
    return x->name;
}

void Treceive_free(t_Treceive* x) {
    if(global_receive_dictionary && x->dictionary_id){
        dictionary_chuckentry(global_receive_dictionary, x->dictionary_id);
    }
}

void* Treceive_new(t_symbol* s, long argc, t_atom* argv) {
	t_Treceive* x = NULL;
    
    x = (t_Treceive*)object_alloc(Treceive_class);
    
	if(x){
        
        x->outlet = outlet_new((t_object *)x, NULL);
        
        if(argc && argv){
            x->name = atom_getsym(argv);
        }else{
            x->name = gensym("");
        }
        
        x->input_message = NULL;
        
        if(global_receive_dictionary){
            char str[64];
            snprintf_zero(str, 64, "receive_%d", global_id++);
            x->dictionary_id = gensym(str);
            dictionary_appendobject(global_receive_dictionary, x->dictionary_id, (t_object *)x);
        }
	}
	return (x);
}
