// De Meo Marco 856627 m.demeo@campus.unimib.it

/**
  @file transaction_model.h
  
  @brief File header della classe transaction_model

  File che contiene le dichiarazioni del modello di transazione per la visualizzazione tabellare
 */
#ifndef TRANSACTION_MODEL_H
#define TRANSACTION_MODEL_H

#include <QDate>
#include <QString>
#include <QAbstractTableModel>
#include <QVector>

/**
  @brief Struttura dati Transaction

  Struttura dati Transaction interna.
  Contiene i campi data, descrizione, categoria e importo.
 */
struct Transaction {
    QDate data;
    QString descrizione;
    QString categoria;
    double importo;

    /**
     @brief uguaglianza
     Confronta due transazioni campo per campo

     @param other struct Transaction da confrontare
     */
    bool operator==(const Transaction& other) const;
    
    /**
     @brief diversità
     Implementato come negazione del ==

     @param other struct Transaction da confrontare
     */
    bool operator!=(const Transaction& other) const;
};

/** 
  @brief Classe TransactionModel
  
  La classe è un'estensione di QAbstractTableModel,
  subclass di QAbstractItemModel per tabelle 2D,
  comunemente usata per tabelle in views come QTableView.
  Utilizzo QTableView invece di un QTableWidget
  perché devo definire io add/edit/delete,
  offre un metodo di filtering semplice,
  ed è semplice da legare ai grafici.
 */
class TransactionModel : public QAbstractTableModel {
    Q_OBJECT
public:
    /**
      @brief Costruttore della classe TransactionModel 

      Costruttore della classe TransactionModel.
      Inizializza a filtro nullo, inizialmente mostra tutte le date.
      La classe è un'estensione di QAbstractTableModel, a sua volta derivata di QAbstractItemModel
      Le funzioni rowCount, columnCount, data e headerData 
      Sono override di metodi virtuali di QAbstracItemModel 

      @param parent il QObject parent che di default è nullptr 
    */
    explicit TransactionModel(QObject *parent = nullptr);

    
    /**
       @brief Funzione di input data da file CSV

      Funzione di input data da file CSV
      Include controllo sul formato dei dati inseriti.
      Chiamata da un button nella GUI.
      Può far visualizzare un messaggio di errore in un dialog.

      @param filename filepath del file da leggere
      @param error stringa sulla quale viene spedito il messaggio di errore

      @return true se effettua il caricamento, false in caso di errore
    */
    bool loadFromCSV(const QString &filename, QString &errorMsg);

    
    /**
      @brief Funzione di output data su file CSV
 
      Funzione di output data su file CSV.
      Chiamata da un button nella GUI.
      Può far visualizzare un messaggio di errore in un dialog.

      @param filename filepath del file su cui scrivere
      @param error stringa sulla quale viene spedito il messaggio di errore

      @return true se effettua la scrittura, false in caso di errore
    */
    bool saveToCSV(const QString &filename, QString &errorMsg);

    
    /**
      @brief Funzione che passa alla QTableView il numero di righe da mostrare

      Funzione che passa alla QTableView il numero di righe da mostrare

      @param parent per gerarchia dei modelli, non utilizzato in questo caso

      @return il numero di righe da mostrare
    */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    
    
    /**
      @brief Funzione che passa alla QTableView il numero di colonne da mostrare
    
      Funzione che passa alla QTableView il numero di colone da mostrare
      In questo caso il numero è fisso, perché vogliamo un formato specifico
      Ma la funzione fa parte della specifica del framework Qt

      @param parent per gerarchia dei modelli, non utilizzato in questo caso

      @return il numero di colonne da mostrare
    */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    

    /**
      @brief Funzione che passa alla QTableView i dati della specifica cella
    
      Funzione che passa alla QTableView i dati della specifica cella

      @param index indice della cella riferita
      @param role identifica l'operazione, è una specifica del framework Qt

      @return dato all'indice index in un QVariant, container generico, per flessibilità
    */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    
    /**
      @brief Funzione che passa i titoli delle colonne alla QTableView

      Funzione chiamata dalla view della tabella.
      Per displayare i titoli delle colonne.

      @param index indice della colonna passata
      @param orientation orientation per dare riga/colonna
      @param role identifica l'operazione, è una specifica del framework Qt

      @return il dato wrappato in un QVariant (container generico, x flessibilità)
    */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    
    /**
      @brief Funzione getter di una transazione 
      
      Restituisce la transazione alla riga filtrata richiesta
      Funzione helper per gli slot che operano sulle transazioni nella mainwindow 

      @param row indice della riga nella vista filtrata
      @return la transazione corrispondente
    */
    Transaction getTransaction(int row) const;



    /**
      @brief Funzione per aggiunta di una transazione al modello

      Funzione per l'aggiunta di una transazione al modello

      @param t oggetto transazione da aggiungere
    */
    void addTransaction(const Transaction &t);
    
    
    /**
      @brief Funzione per la modifica di una transazione

      Funzione per la modifica di una transazione
      Usa il signal dataChanged di QAbstractItemModel
    
      @param row riga alla quale si trova la transazione da modificare
      @param t oggetto transazione che sostituirà la transazione da modificare
    */
    void editTransaction(int row, const Transaction &t);
    
    
    /**
      @brief Funzione che rimuove le transazioni selezionate

      Funzione che rimuove le transazioni selezionate
      Passo un QModelIndexList, è il tipo usato dalle views quando si selezionano più righe
      QModelIndex è un oggetto complesso che contiene molti dati
      --> estraggo gli indici di riga in rowNumbers

      @param rows lista di transazioni da eliminare
    */
    void removeTransactions(const QModelIndexList &rows);

    
    /**
      @brief Funzione che imposta il filtro x intervallo date

      Funzione che imposta il filtro per le transazioni con un intervallo di date

      @param from data di partenza filtro
      @param to data di termine filtro
    */
    void setDateFilter(const QDate &from, const QDate &to);

private:
    QVector<Transaction> m_transactions; // tutte le transazioni
    QVector<Transaction> m_filtered;     // transazioni filtrate
    QDate m_filterFrom, m_filterTo;      // intervallo di filtro
    
    /**
      @brief Funzione helper che applica il filtro alla lista delle transazioni

      Funzione helper che applica il filtro alla lista delle transazioni
      Dichiarata private perché usata solo internamente
    */
    void updateFilter();

}; // class TransactionModel

#endif
