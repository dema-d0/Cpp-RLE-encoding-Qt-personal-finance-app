// De Meo Marco 856627 m.demeo@campus.unimib.it

/**
  @file main.cpp

  @brief File di test per la classe RLE templata
*/
#include "rle.h"
#include <iostream>
#include <cassert>    // assert
#include <vector>     // std::vector
#include <list>       // std::list

    
// FUNTORI:

/**
  @brief Funtore per il confronto uguaglianza tra due interi.
*/
struct int_equal {
    bool operator()(int a, int b) const {
        return (a == b);
    }
};

/**
  @brief Funtore per il confronto uguaglianza tra due double.
*/
struct double_equal {
    bool operator()(double a, double b) const {
        return (a == b);
    }
};

/**
  @brief Funtore per il confronto uguaglianza tra due char.
*/
struct char_equal {
    bool operator()(char a, char b) const {
        return (a == b);
    }
};

/**
  @brief Funtore per il confronto uguaglianza tra due bool.
*/
struct bool_equal {
    bool operator()(bool a, bool b) const {
        return (a == b);
    }
};

/**
  Funtore per il confronto tra stringhe. 
  True se le stringhe sono uguali. 

  @brief Funtore per il confronto uguaglianza tra stringhe.
*/
struct string_eql {
  bool operator()(const std::string &a, const std::string &b) const {
    return (a == b);
  } 
};
    
    

/**
  Test dei metodi con tipi primitivi

  @brief Test dei metodi con tipi primitivi
*/
void test_tipi_primitivi() {
    
  std::cout << "Chiamate al default ctor" << std::endl;
  RLE<int, int_equal> int_rle;
  RLE<double, double_equal> db_rle;
  RLE<char, char_equal> char_rle;
  RLE<bool, bool_equal> bool_rle;

  int dati[10] = {5,5,3,3,2,1,1,1,8,4};

  RLE<int, int_equal> rle_arr_int(dati, dati+10);
  std::cout << rle_arr_int << std::endl;
  std::cout << "rle_arr_int.size(): " <<  rle_arr_int.size() << std::endl;
  assert(rle_arr_int.size() == 6);

  RLE<double, double_equal> rle_arr_db(dati, dati+10);
  std::cout  << rle_arr_db << std::endl;
  std::cout  << "rle_arr_db.size(): "<< rle_arr_db.size() << std::endl;
  assert(rle_arr_db.size() == 6);

  std::vector<int> intvec = {1,1,2,3,3,3,4,4,4,4,0,4,4,4,5,5,6,7,7};
  std::cout << "Test sequenza di interi, creazione e stampa" << std::endl;
  RLE<int, int_equal> rle_vec_int(intvec.begin(), intvec.end());
  std::cout << rle_vec_int << std::endl;
  std::cout << "rle_vec_int.size(): " <<  rle_vec_int.size() << std::endl;

  std::cout << "Test sequenza di interi, encode e stampa" << std::endl;
  rle_vec_int.encode(intvec.begin(), intvec.end());
  std::cout << rle_vec_int << std::endl;
  std::cout << "rle_vec_int.size(): " <<  rle_vec_int.size() << std::endl;
  
  std::list<char> chlist1 = {'a','a','a','b','c','c','c','c','d','d','a'};
  std::cout << "Test sequenza di interi, cast da char e stampa" << std::endl;
  rle_vec_int.encode(chlist1.begin(), chlist1.end());
  std::cout << rle_vec_int << std::endl;
  std::cout << "rle_vec_int.size(): " <<  rle_vec_int.size() << std::endl;
  
  std::cout << "Test sequenza di interi, cast encode da char e stampa" << std::endl;
  rle_vec_int.encode(chlist1.begin(), chlist1.end());
  std::cout << rle_vec_int << std::endl;
  std::cout << "rle_vec_int.size(): " <<  rle_vec_int.size() << std::endl;

  std::vector<double> dbvec = {1.0, 1.0, 2.6, 2.6, 2.6, 3.0};

  std::cout << "Test 1 sequenza di char, cast da double e stampa" << std::endl;
  RLE<char, char_equal> rle_vec_char(dbvec.begin(), dbvec.end());
  std::cout << rle_vec_char << std::endl;
  std::cout << "rle_vec_char.size(): " <<  rle_vec_char.size() << std::endl;
  
  std::cout << "Test 2 sequenza di char, encode char post cast da double e stampa" << std::endl;
  rle_vec_char.encode(chlist1.begin(), chlist1.end());
  std::cout << rle_vec_char << std::endl;
  std::cout << "rle_vec_char.size(): " <<  rle_vec_char.size() << std::endl;

  std::cout << "Test 3 sequenza di char, cast encode da int e stampa" << std::endl;
  rle_vec_char.encode(intvec.begin(), intvec.end());
  std::cout << rle_vec_char << std::endl;
  std::cout << "rle_vec_char.size(): " <<  rle_vec_char.size() << std::endl;

  std::cout << "Test 1 sequenza di double, cast da int e stampa" << std::endl;
  RLE<double, double_equal> rle_vec_dbl(intvec.begin(), intvec.end());
  std::cout << rle_vec_dbl << std::endl;
  std::cout << "rle_vec_dbl.size(): " <<  rle_vec_dbl.size() << std::endl;

  std::cout << "Test 2 sequenza di double, encode da double e stampa" << std::endl;
  rle_vec_dbl.encode(dbvec.begin(), dbvec.end());
  std::cout << rle_vec_dbl << std::endl;
  std::cout << "rle_vec_dbl.size(): " <<  rle_vec_dbl.size() << std::endl;

  std::cout << "Test 3 sequenza di double, cast encode da char e stampa" << std::endl;
  rle_vec_dbl.encode(chlist1.begin(), chlist1.end());
  std::cout << rle_vec_dbl << std::endl;
  std::cout << "rle_vec_dbl.size(): " <<  rle_vec_dbl.size() << std::endl;
  
  std::list<bool> boolist = {true, true, false, false, false, true};
  std::cout << "Test sequenza di bool, creazione e stampa" << std::endl;
  std::cout << "(stampa 1 per TRUE e 0 per FALSE)" << std::endl;
  RLE<bool, bool_equal> rle_bool_list(boolist.begin(), boolist.end()); 
  std::cout << rle_bool_list << std::endl;
  std::cout << "rle_bool_list.size(): " <<  rle_bool_list.size() << std::endl;

  
  std::cout << "Chiamate al copy ctor: " << std::endl;

  std::cout << "Copy ctor sequenza int" << std::endl;
  RLE<int, int_equal> int_rle2(rle_vec_int);
  std::cout << int_rle2 << std::endl;

  std::cout << "Copy ctor sequenza double" << std::endl;
  RLE<double, double_equal> db_rle2(rle_vec_dbl);
  std::cout << db_rle2 << std::endl;
  
  std::cout << "Copy ctor sequenza char" << std::endl;
  RLE<char, char_equal> char_rle2(rle_vec_char);
  std::cout << char_rle2 << std::endl;
  
  std::cout << "Copy ctor sequenza bool" << std::endl;
  RLE<bool, bool_equal> bool_rle2(rle_bool_list);
  std::cout << bool_rle2 << std::endl;


  std::cout << "Chiamate all'operatore di assegnamento" << std::endl;
  
  std::cout << "Operatore di assegnamento int" << std::endl;
  RLE<int, int_equal> int_rle3;
  int_rle3 = int_rle2;
  std::cout << int_rle3 << std::endl;
  
  std::cout << "Operatore di assegnamento double" << std::endl;
  RLE<double, double_equal> db_rle3;
  db_rle3 = db_rle2;
  std::cout << db_rle3 << std::endl;
  
  std::cout << "Operatore di assegnamento char" << std::endl;
  RLE<char, char_equal> char_rle3;
  char_rle3 = char_rle2;
  std::cout << char_rle3 << std::endl;
  
  std::cout << "Operatore di assegnamento bool" << std::endl;
  RLE<bool, bool_equal> bool_rle3;
  bool_rle3 = bool_rle2;
  std::cout << bool_rle3 << std::endl;
}    


/**
  Test d'uso della RLE costante passata come parametro

  @brief Test d'uso della RLE costante

  @param rle RLE da usare per i test
 */
void test_const_rle_int(const RLE<int, int_equal> &rle) {
  std::cout << "Test su una RLE costante di interi" << std::endl;

  std::cout << "Stampa compatta tramite operator di stream: "<<std::endl;
  std::cout << rle << std::endl;

  std::cout <<"rle.size(): "<< rle.size() <<std::endl;

  RLE<int,int_equal>::const_iterator curr, end;

  std::cout << "Stampa sequenza espansa tramite iteratori: " << std::endl;
  for(curr = rle.begin(), end = rle.end(); curr != end; ++curr)
    std::cout<<*curr<<std::endl;
  
}


/** 
  Test della RLE costruita con sequenze di stringhe

  @brief Test della RLE costruita con sequenze di stringhe
*/
void test_stringhe() {
  std::cout << "Test con sequenze di stringhe" << std::endl;
  
  // Prima sequenza di stringhe
  std::vector<std::string> strings1 = {
    "pippo", "pippo", "pippo", "pluto",
    "duffy", "duffy", "bunny", "pippo"
  };

  // Costruzione della RLE con la prima sequenza
  RLE<std::string, string_eql> string_rle(strings1.begin(), strings1.end());   
  std::cout << "Stampa tramite operator<<" << std::endl;
  std::cout << string_rle << std::endl;
  
  std::cout << "Stampa sequenza espansa tramite RLE::const_iterator" << std::endl;
  RLE<std::string, string_eql>::const_iterator curr, end;
  for(curr = string_rle.begin(), end = string_rle.end(); curr != end; ++curr)
    std::cout << *curr << " ";
  std::cout << std::endl;
  
  std::cout << "string_rle.size(): " << string_rle.size() << std::endl;

  // Seconda sequenza di stringhe, x encoding in coda
  std::vector<std::string> strings2 = {
    "mickey", "pietro", "pietro", "pietro", "minnie", "minnie"
  };

  std::cout << "Encoding in coda di altri punti:" << std::endl;
  string_rle.encode(strings2.begin(), strings2.end());

  std::cout << "Stampa nuova sequenza tramite operator<<" << std::endl;
  std::cout << string_rle << std::endl;

  std::cout << "Stampa nuova sequenza espansa tramite RLE::const_iterator" << std::endl;
  for(curr = string_rle.begin(), end = string_rle.end(); curr != end; ++curr)
    std::cout << *curr << " ";
  std::cout << std::endl;
  
  std::cout << "string_rle.size(): " << string_rle.size() << std::endl;

}

/**
  Struct point che implementa un punto 3D.

  @brief Struct point che implementa un punto 3D.
*/
struct point {
  int x; ///< coordinata x del punto
  int y; ///< coordinata y del punto
  int z; ///< coordinata z del punto

  point(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
};

/**
  Ridefinizione dell'operatore di stream << per un punto 3D.
  Necessario per l'operatore di stream della classe RLE.
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
  std::cout << "(" << p.x << "," << p.y << "," << p.z << ")";
  return os;
}

/**
  @brief Funtore per il confronto di due punti 3D.
  
  Funtore per il confronto di due punti 3D. 
  Ritorna true se tutte le coordinate sono uguali
*/
struct point_eql {
  bool operator()(const point &p1, const point &p2) const {
    return ((p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z));
  } 
};


/**
  Test della RLE istanziata su oggetti 3D point

  @brief Test della RLE istanziata su oggetti 3D point
*/
void test_RLE_3Dpoints() {

  std::cout << "Test su una RLE di punti 3D" << std::endl;

  // Prima sequenza di punti 3D
  std::vector<point> points1 = {
    point(1,2,3), point(1,2,3), point(4,5,6), point(4,5,6),
    point(4,5,6), point(7,8,9), point(1,2,3)
  };

  // Costruzione della RLE con la prima sequenza
  RLE<point, point_eql> points_rle(points1.begin(), points1.end());   
  std::cout << "Stampa tramite operator<<" << std::endl;
  std::cout << points_rle << std::endl;
  
  std::cout << "Stampa sequenza espansa tramite RLE::const_iterator" << std::endl;
  RLE<point, point_eql>::const_iterator curr, end;
  for(curr = points_rle.begin(), end = points_rle.end(); curr != end; ++curr)
    std::cout << *curr << " ";
  std::cout << std::endl;
  
  std::cout << "points_rle.size(): " << points_rle.size() << std::endl;

  // Seconda sequenza di punti 3D, x encoding in coda
  std::vector<point> points2 = {
    point(7,8,9), point(7,8,9), point(10,11,12), point(1,2,3)
  };

  std::cout << "Encoding in coda di altri punti:" << std::endl;
  points_rle.encode(points2.begin(), points2.end());

  std::cout << "Stampa nuova sequenza tramite operator<<" << std::endl;
  std::cout << points_rle << std::endl;

  std::cout << "Stampa nuova sequenza espansa tramite RLE::const_iterator" << std::endl;
  for(curr = points_rle.begin(), end = points_rle.end(); curr != end; ++curr)
    std::cout << *curr << " ";
  std::cout << std::endl;
  
  std::cout << "points_rle.size(): " << points_rle.size() << std::endl;

}


/**
  Test di uso dell'iteratore proprietario di RLE che ritorna la sequenza scompattata

  @brief Test d'uso del const_iterator che opera su sequenza espansa
*/
void test_iteratore_sequenza_espansa() {
  std::cout << "Test di stampa della sequenza espansa" << std::endl;

  std::vector<int> vettore = {1,1,1,2,2,5,3,3,3,3,8,8,9,7,7,7};
  
  RLE<int, int_equal> rle_v(vettore.begin(), vettore.end());
  // stampa della sequenza compatta tramite operatore di stream
  std::cout << "Stampa della sequenza compatta tramite operator<<" << std::endl;
  std::cout << rle_v << std::endl;
  std::cout << "rle_v.size(): " << rle_v.size() << std::endl;

  // istanzio due iteratori dalla classe iteratore definita in RLE
  RLE<int, int_equal>::const_iterator curr, end;

  // stampa della sequenza espansa tramite RLE::const_iterator
  std::cout << "Stampa della sequenza espansa tramite RLE::const_iterator" << std::endl;
  for(curr = rle_v.begin(), end = rle_v.end(); curr != end; ++curr)
      std::cout << *curr << " ";
  std::cout << std::endl;
  std::cout << "rle_v.size(): " << rle_v.size() << std::endl;

}


int main() {
  
  test_tipi_primitivi();  
  test_stringhe();        
  test_RLE_3Dpoints();    
  test_iteratore_sequenza_espansa();
  
  RLE<int, int_equal> rle;
  test_const_rle_int(rle);

  return 0;
}