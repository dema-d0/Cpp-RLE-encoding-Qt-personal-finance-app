// De Meo Marco 856627 m.demeo@campus.unimib.it

/**
  @file transaction_model.cpp
  
  @brief File sorgente della classe transaction_model

  File che contiene il modello di transazione per la visualizzazione tabellare
 */
#include "transaction_model.h"
#include <QAbstractTableModel> // classe padre del mio modello
#include <QFile>               // gestione file
#include <QTextStream>         // wrapper I/O per QFile 
#include <QDate>               // date + utilities
#include <QString>
#include <QVector>             // vec di Transaction + utilities varie
#include <algorithm>           // std::sort
#include <functional>          // std::greater - funtore usato nel sort 

// uguaglianza
bool Transaction::operator==(const Transaction& other) const {
    return ((data == other.data) && (descrizione == other.descrizione) 
            && (categoria == other.categoria) && (importo == other.importo));
}

// diversità
bool Transaction::operator!=(const Transaction& other) const {
    return !(other == *this);
}

// default ctor
TransactionModel::TransactionModel(QObject *parent) : QAbstractTableModel(parent) {
    // inizializza a date null
    m_filterFrom = QDate(); 
    m_filterTo = QDate();   
    // init della lista transazioni filtrate
    m_filtered.clear();
}

// input da CSV
bool TransactionModel::loadFromCSV(const QString &filename, QString &error) {
    QFile file(filename);
    // errore apertura fle in lettura
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = "Errore nell'apertura del file in lettura";
        return false;
    }
    
    // oggetto QTextStream per lettura di contenuto testuale
    QTextStream in(&file);
    // vettore di appoggio per transazioni parsate
    QVector<Transaction> temp;
    // variabile che contiene la nRiga del file a cui avviene l'errore
    int nRiga = 0;
    // cicla fino a fine file e parsa riga per riga,
    // divide i campi con una virgola
    while(!(in.atEnd())) {
        QString riga = in.readLine();
        nRiga++;
        // QStringList == QList<QString>
        // ma è + compatibile con le funzioni di handling testo
        QStringList fields = riga.split(',');
        // controlla il formato dei campi della transazione
        if(fields.size() != 4) {
            error = QString("Formato non valido alla riga %1").arg(nRiga);
            return false;
        }
        // parsa il campo data 
        QDate date = QDate::fromString(fields[0], "yyyy-MM-dd");
        if(!date.isValid()) {
            error = QString("Data non valida alla riga %1").arg(nRiga);
            return false;
        }
        // parsa il campo importo
        bool ok = false;
        double importo = fields[3].toDouble(&ok);
        if(!ok) {
            error = QString("Importo non valido alla riga %1").arg(nRiga);
            return false;
        }
        // aggiunge in coda al vettore un oggetto Transaction con i campi
        temp.append(Transaction{date, fields[1], fields[2], importo});
    }
    
    // disconnette il model dalle view
    beginResetModel();
    // aggiorna le transazioni nel modello
    m_transactions = temp;
    // aggiorna le transazioni
    updateFilter();
    // riconnette il model alle view
    endResetModel();
    
    return true;
}

// output su CSV
bool TransactionModel::saveToCSV(const QString &filename, QString &error) {
    QFile file(filename);
    // errore apertura file in scrittura
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        error = "Errore nell'apertura del file in scrittura";
        return false;
    }
    // oggetto QTextStream per scrittura di contenuto testuale
    QTextStream out(&file);
    // cicla su tutte le transazioni nel modello
    for(const Transaction &t : m_transactions) {
        // scrive i campi separati da virgole
        // QDate::toString
        out << t.data.toString("yyyy-MM-dd") << ","
            << t.descrizione << ","
            << t.categoria << ","
            // porta il campo importo da numero a stringa
            // con 2 cifre decimali
            // (double non ha toString)
            << QString::number(t.importo, 'f', 2) << "\n";
    }
    return true;
}

// conta le righe da mostrare
// override da QAbstractItemModel
int TransactionModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_filtered.size();
}

// conta le colonne da mostrare
// override da QAbstractItemModel
int TransactionModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 4; // nel nostro caso il formato è fisso
}

// dati da mostrare
// override da QAbstractItemModel
QVariant TransactionModel::data(const QModelIndex &index, int role) const {
    // in caso di posizione non valida -> QVariant vuoto
    if(!index.isValid() || index.row() < 0 || index.row() >= m_filtered.size())
        return QVariant();
    // seleziona la transazione alla riga che corrisponde a index
    const Transaction &t = m_filtered[index.row()];
    if(role == Qt::DisplayRole) {
        // in base alla colonna di index tratta il campo in maniera diversa
        switch (index.column()) {
        // Explicit dd/MM/yyyy format - force European format
        case 0: {
            int day = t.data.day();
            int month = t.data.month();
            int year = t.data.year();
            QString dateStr = QString("%1/%2/%3")
                      .arg(day, 2, 10, QChar('0'))
                      .arg(month, 2, 10, QChar('0'))
                      .arg(year, 4, 10, QChar('0'));
            qDebug() << "TransactionModel::data: Returning date string:" << dateStr;
            return dateStr;
        }
        case 1: return t.descrizione;
        case 2: return t.categoria;
        // porta il campo importo da numero a stringa
        // con 2 cifre decimali (double non ha toString)
        case 3: return QString::number(t.importo, 'f', 2);
        }
    }
    // se non entra nello switch -> QVariant vuoto
    return QVariant();
}

// titoli colonna da mostrare
// override da QAbstractItemModel
QVariant TransactionModel::headerData(int index, Qt::Orientation orientation, int role) const {
    if(role != Qt::DisplayRole)
        return QVariant();
    // orientation orizzontale <=> titoli sono di colonna
    if(orientation == Qt::Horizontal) {
        switch (index) {
        case 0: return "Data";
        case 1: return "Descrizione";
        case 2: return "Categoria";
        case 3: return "Importo";
        }
    }
    // se non entra nello switch ritorna un QVariant vuoto
    return QVariant();
}

// getter della transazione - helper
Transaction TransactionModel::getTransaction(int row) const {
    // se la riga non è valida ritorna una transazione vuota
    if (row < 0 || row >= m_filtered.size())
        return Transaction{};
    // altrimenti ritorna la transazione alla riga selezionata
    return m_filtered[row];
}

// aggiunta transazione al modello
void TransactionModel::addTransaction(const Transaction &t) {
    // scollega il model dalle view
    beginResetModel();
    // aggiunge al vettore delle transazioni t
    m_transactions.append(t);
    // aggiorna le transazioni
    updateFilter();
    // ricollega il model alle view
    endResetModel();
}

// modifica transazione nel modello
void TransactionModel::editTransaction(int row, const Transaction &t) {
    // se si passa una riga non valida per la modifica
    if(row < 0 || row >= m_filtered.size()) return;
    // trova l'indice nella lista completa, se passo da quella filtrata
    int realIndex = m_transactions.indexOf(m_filtered[row]);
    if(realIndex >= 0) {
        // trova la transazione e la aggiorna
        m_transactions[realIndex] = t;
        // aggiorna le transazioni
        updateFilter();
        // signal che comunica alla view dela avvenuta modifica 
        // --> aggiorna tutte le colonne
        emit dataChanged(index(row, 0), index(row, columnCount() - 1));
    }
}

// rimuovi una transazione dal modello
void TransactionModel::removeTransactions(const QModelIndexList &rows) {
    // lista per gli indici di riga
    QList<int> rowNumbers;
    for(unsigned int i = 0; i < rows.size(); ++i)
        rowNumbers.append(rows.at(i).row());
    // riordina gli indici in ordine decrescente, se ho selezionato in maniera disordinata
    std::sort(rowNumbers.begin(), rowNumbers.end(), std::greater<int>());
    for(unsigned int r = 0; r < rowNumbers.size(); ++r) {
        int row = rowNumbers[r];
        // controllo se la riga è valida
        if(row >= 0 && row < m_filtered.size()) {
            // trova l'indice nella lista completa, se passo da quella filtrata
            int realIndex = m_transactions.indexOf(m_filtered[row]);
            if(realIndex >= 0) {
                // scollega il model dalle view
                beginResetModel();
                // rimuove la trnsazione
                m_transactions.removeAt(realIndex);
                updateFilter();
                // ricollega il model alle view
                endResetModel();
            }
        }
    }
}

// applica un filtro al modello
void TransactionModel::setDateFilter(const QDate &from, const QDate &to) {
    m_filterFrom = from;
    m_filterTo = to;
    
    // Notify the view that the model structure is about to change
    beginResetModel();
    updateFilter(); // applico il filtro
    // Notify the view that the model structure has changed
    endResetModel();
}

// helper della setDateFilter che applica effettivamente il filtro
void TransactionModel::updateFilter() {
    // svuota l'attuale lista (QVector) di transazioni filtrata
    m_filtered.clear();
    // cicla sulla lista completa (non filtrata) di transazioni
    for(unsigned int i = 0; i < m_transactions.size(); ++i) {
        const Transaction &t = m_transactions[i];
        bool inRange = true;
        // se ho settato 'from' ma la data della transazione è precedente
        if(m_filterFrom.isValid() && t.data < m_filterFrom)
            inRange = false;
        // se ho settato 'to' ma la data della transazione è successiva
        if(m_filterTo.isValid() && t.data > m_filterTo)
            inRange = false;
        // altrimenti la data della transazione[i] va bene
        // --> aggiungo la transazione [i] alla lista filtrata
        if(inRange)
            m_filtered.append(t);
    }
}
