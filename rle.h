// De Meo Marco 856627 m.demeo@campus.unimib.it

/** 
  @file rle.h

  @brief File header della classe RLE templata

  File di dichiarazioni/definizioni della classe RLE templata
*/
#ifndef RLE_H
#define RLE_H

#include <ostream>   // operator<<
#include <iostream>  // debug codeblocks
#include <algorithm> // std::swap

#include <iterator>  // std::forward_iterator_tag
#include <cstddef>   // std::ptrdiff_t


/**
  @brief Classe per la compressione di sequenze dati tramite RLE

  La classe implementa una sequenza di dati generici T
  conservati nella loro forma compressa tramite RLE.
  Il controllo di uguaglianza tra i dati è delegato a un funtore Eql
  che prende due valori di tipo T e ritorna true sse sono uguali.
  La compressione, anche in fase di costruzione degli oggetti,
  è affidata alla funzione di encoding.

*/
template <typename T, typename Eql> // Comp funct x uguaglianza tra V
class RLE {

public:   
    typedef T value_type;
    typedef unsigned int size_type;

private:

    /**
      @brief Struttura dati pair

      Struttura dati pair interna, che conserva le coppie <V, N>.
    */
    struct pair {
        value_type _value;    ///< valore V
        size_type _repCount;  ///< numero di ripetizioni N
        pair *_next;          ///< puntatore al pair successivo

        // non posso inizializzare value a un valore di default perché è templato
        // --> quando mi dovesse servire lascio la scelta al chiamante con un parametro 'init'

        /**
          Costruttre di default
          @post _repCount == 0
          @post _next == nullptr
        */
        pair() : _repCount(0), _next(nullptr) {}

        /**
          Costruttore secondario
          @param v valore V da copiare
          @param rc numero di ripetizioni N da copiare
          @param p puntatore al prossimo pair

          @post _value == v
          @post _repCount == rc
          @post _next == p
        */
        pair(const value_type &v, const size_type &rc, pair * p) 
        : _value(v), _repCount(rc), _next(p) {}
        
        /**
          Costruttore secondario
          @param v valore V da copiare
          @param rc numero di ripetizioni N da copiare

          @post _value == v
          @post _repCount == rc
          @post _next == nullptr
        */
        pair(const value_type &v, const size_type &rc) 
        : _value(v), _repCount(rc), _next(nullptr) {}

        
        /**
          Costruttore secondario
          @param v valore V da copiare

          @post _value == v
          @post _repCount == 1
          @post _next == nullptr
        */
        explicit pair(const value_type &v) 
        : _value(v), _repCount(1), _next(nullptr) {}

        // I restanti metodi potrebbero essere omessi
        // perché sono quelli generati di default dal compilatore

        /**
          Copy constuctor 
          
          Marcato explicit per evitare conversioni implicite indesiderate

          @param other pair da copiare 
        */
        pair(const pair &other) {
            _value = other._value;
            _repCount = other._repCount;
            _next = other._next;
        }

        /**
          Distruttore
        */
        ~pair() {}

        /**
          Operatore di assegnamento

          @param other pair da copiare

          @return reference al pair this
        */
        pair &operator=(const pair &other) {
            _value = other._value;
            _repCount = other._repCount;

            return *this;
        }


    }; // struct pair
    
    pair *_headens;  ///< puntatore al primo pair della classe
    pair *_tailens;  ///< puntatore all'ultimo pair della classe
    size_type _size; ///< dimensione della sequenza (# pairs)
    Eql _eqcompare;  ///< funtore binario per uguaglianza

public:
    
    /**
      Costruttore di default.
      
      @post _headens == nullptr
      @post _tailens == nullptr
      @post _size == 0
    */
    RLE() : _headens(nullptr), _tailens(nullptr), _size(0) {
        #ifndef NDEBUG
        std::cout << "RLE::RLE()" << std::endl;
        #endif    
    }


    /**
      Costruttore che crea una RLE a partire da una sequenza,
      rappresentata da un iteratore di inizio e uno di fine.
      Prende i dati della sequenza in ingresso,
      e li conserva in forma compressa costruendo la RLE
      con la funzione di encoding.
      Gli iteratori possono essere di qualunque tipo,
      conversione delegata al compilatore.
      
      @tparam Q tipo degli iteratori in input
      @param begin iteratore di inizio sequenza
      @param end iteratore di fine sequenza

      @throw std::bad_alloc possibile errore di allocazione dalla chiamata a encode
    */
    template <typename Q>
    RLE(Q begin, Q end) 
    : _headens(nullptr), _tailens(nullptr), _size(0) { 
        // templata su Q, 
        // calls append() 
        // implementa static_cast policy 
        encode(begin, end); 

        #ifndef NDEBUG
        std::cout << "RLE::RLE(Q begin, Q end)" << std::endl;
        #endif
    }    

    
    /**
      Funzione di encoding.
      Prende in input una sequenza di dati rappresentata
      da un iteratore di inizio e uno di fine.
      Gli iteratori possono essere di qualunque tipo,
      conversione delegata al compilatore.

      Può essere utilizzata per la costruzione di una nuova RLE,
      oppure per l'aggiunta di nuovi dati codificati
      in coda a quelli già presenti in un'istanza di RLE.
      
      @tparam Q tipo degli iteratori in input
      @param begin iteratore di inizio sequenza
      @param end iteratore di fine sequenza
      
      @throw std::bad_alloc possibile error
    */
    template <typename Q>
    void encode(Q begin, Q end) {
        // x efficienza e per evitare segmentation fault
        // che sarebbe causata da *nullptr in *begin
        if(begin == end) return;    

        size_type count = 0;       ///< init count reps
        value_type value = 
        static_cast<T>(*begin);    ///< init value to compare in the seq
        pair * gc = _tailens;      ///< garbage collection ptr, 
                                   ///  qui a _tailens se fallisce encode su RLE parzialmente piena
        try {
            for(; begin != end; ++begin) {
                // se il valore in copia e quello nella sequenza
                // sono uguali --> aumento il count
                if(_eqcompare(value, *begin)) 
                    count++;
                // se sono diversi scarico il pair nella RLE e resetto
                else { 
                    append(value, count);
                    value = static_cast<T>(*begin);
                    count = 1;
                }
            }
            // aggiunta dell'ultimo pair
            // per la logica del for sopra altimenti non verrebbe aggiuunto
            append(value, count);
        } catch(...) {
            // perché se è diverso da nullptr sono sicuro
            // che la classe aveva già una sequenza
            // --> non la voglio cancellare
            if(gc != nullptr)
                // gli passo il next, primo pair aggiunto in coda
                // altrimenti mi cancella anche la tail della RLE 
                // già presente prima dell'esecuzione del metodo
                clearRLE(gc->_next);
            else
                clearRLE(_headens);
            throw;
        }

        #ifndef NDEBUG
        std::cout << "RLE::encode(Q begin, Q end, eql)" << std::endl;
        #endif
    }


    /**
      Aggiunge un pair nella RLE mantenendo i puntatori in uno stato coerente.
      Funzione helper della funzione di encoding.
      
      @param value valore V del pair
      @param count numero N del pair

      @post _size += 1
    */ 
    void append(const T &value, const size_type count) {
        pair *tmp = new pair(value, count); 
        if(_headens == nullptr) {  // in caso di classe RLE vuota
            _headens = tmp;        // set head
            _tailens = tmp;        // set tail
            _size = 1;             // inc size
        } else {                   // append a dati preesistenti
            _tailens->_next = tmp; // set next della tail
            _tailens = tmp;        // set tail
            _size++;               // inc size
        }
        #ifndef NDEBUG
        std::cout << "RLE::append(const &value, const count)" << std::endl;
        #endif
    }


    /**
      Costruttore di copia
      
      @param other RLE da copiare 

      @throw std::bad_alloc possibile errore di allocazione
    */
    RLE(const RLE &other) 
    : _headens(nullptr), _tailens(nullptr), _size(0) {
        // per efficienza, in caso di empty copy
        if(other._headens == nullptr) {
            return;
        }
        // ptr alla testa dell'altra lista, x navigazione
        pair * curr = other._headens;
        try {
           while(curr != nullptr) {
               // creo un nuovo pair
               pair * tmp = new pair(curr->_value, curr->_repCount);
               if(_tailens == nullptr) {
                    _headens = _tailens = tmp;
               } else {
                    // set next della tail (nodo precedente), 
                    // a cui appendo quello nuovo
                    _tailens->_next = tmp;
                    // cambio la coda, diventa il nuovo nodo
                    _tailens = tmp;
               }
               _size++;
               curr = curr->_next;
            }
        } catch(...) {
            // garbage collection dalla head
            clearRLE(_headens);
            throw;
        }

        #ifndef NDEBUG
        std::cout << "RLE::RLE(const RLE&)" << std::endl;
        #endif
    }


    /**
      Funzione di cancellazione dei pair
      
      Prende in input un puntatore al pair da cui inizare a cancellare.
      Così da poter essere usata dal distruttore 
      o per garbage collection in caso di costruzione fallita,
      oppure per garbage collection in caso di codifica in coda fallita.

      @param garbage_collector puntatore al pair da cui partire
     */
    void clearRLE(pair * garbage_collector) {
        pair * nextgc = nullptr;
        while(garbage_collector != nullptr) {
            nextgc = garbage_collector->_next;
            delete garbage_collector; // cancella quello che c'è a destra del suo uguale
            garbage_collector = nextgc;
        }
        _headens = nullptr;
        _tailens = nullptr;
        _size = 0;
    }


    /**
      Distruttore 
      
      @post _headens == nullptr
      @post _size == 0
    */
    ~RLE() {
        clearRLE(_headens);
    }


    /**
      Operatore di assegnamento

      @param other RLE da copiare

      @return reference alla RLE this

      @throw std::bad_alloc possibile eccezione di allocazione
    */
    RLE &operator=(const RLE &other) {
        
        if(this != &other) {
            RLE temp(other);
            this->swap(temp);
        }
        
        return *this;
    }


    /**
      Funzione che scambia lo stato tra l'istanza corrente di RLE
      e quella passata a parametro.
      
      @param other RLE con la quale scambiare lo stato
    */
    void swap(RLE &other) {
        std::swap(this->_headens, other._headens);
        std::swap(this->_tailens, other._tailens);
        std::swap(this->_size, other._size);
        // swap del funtore nel caso avesse stati interni
        std::swap(this->_eqcompare, other._eqcompare);
    }


    /**
      Funzione che ritorna il numero complessivo di dati codificati,
      ovvero il numero di pair contenuti nella RLE.

      @return numero di pair nella RLE
     */
    size_type size() const {
        return this->_size;
    }


    /**
      @brief operatore di stream <<

      Funzione che implementa l'operatore di stream.

      @param os stream di output
      @param rle RLE da spedire sullo stream

      @return lo stream di output
    */
    friend std::ostream &operator<<(std::ostream &os, const RLE &rle) {
        pair *curr = rle._headens;
        while(curr != nullptr) {
            os << curr->_value << " " << curr->_repCount << " ";
            curr = curr->_next;
        }
        return os;
    }

    /**
      Iteratore costante di tipo forward.
      L'iteratore deve ritornare la sequenza dati espansa,
      non contenuta all'interno della classe.
      Per averne una rappresentazione deve tenere conto di N.
      La logica di espansione è contenuta negli operatori di incremento.
    */
    class const_iterator {
			
	public:
    
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;

        // Default constructor
		const_iterator() : ptr(nullptr), currCount(0) { }
		
        // Copy constructor
		const_iterator(const const_iterator &other) 
        : ptr(other.ptr), currCount(other.currCount) { }

        // Operatore di assegnamento
		const_iterator& operator=(const const_iterator &other) {
			ptr = other.ptr;
            currCount = other.currCount;
            return *this;
		}

        // Distruttore
		~const_iterator() { }

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
        reference operator*() const { 
            return ptr->_value;
        }

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(ptr->_value);
		}
		
		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
            
            const_iterator tmp(*this);
            if(--currCount == 0) {        
                ptr = ptr->_next;
                // necessario per evitare segmentation fault
                // causata da dereferencing di nullptr
                if(ptr)
                currCount = ptr->_repCount;
            }
            return tmp;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			
            if(--currCount == 0) {        
                ptr = ptr->_next;
                // necessario per evitare segmentation fault
                // causata da dereferencing di nullptr
                if(ptr) 
                currCount = ptr->_repCount;
            }
            return *this;
		}

		// Uguaglianza
		bool operator==(const const_iterator &other) const {
			return((ptr == other.ptr) && (currCount == other.currCount));
		}
		
		// Diversita'
		bool operator!=(const const_iterator &other) const {
			return !(other == *this);
		}

		
	private:                               

    const pair *ptr;     ///< Puntatore a struct pair
    size_type currCount; ///< Variabile di appoggio 
                         ///  per iterazione su sequenza espansa

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class RLE; 

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
    // check p!= nullptr prima di dereferenziarlo
		const_iterator(const pair *p) :
        ptr(p), currCount(p ? p->_repCount : 0) {}
		
		
	}; // classe const_iterator
	
	// Ritorna l'iteratore all'inizio della sequenza dati
	const_iterator begin() const {
		return const_iterator(_headens);
	}
	
	// Ritorna l'iteratore alla fine della sequenza dati
	const_iterator end() const {
		return const_iterator(nullptr);
	}

}; // class RLE

#endif