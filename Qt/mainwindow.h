// De Meo Marco 856627 m.demeo@campus.unimib.it

/**
  @file mainwindow.h
  
  @brief File header della main window

  File che contiene le dichiarazioni di classi e metodi per la visualizzazione della main window
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "transaction_model.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QTableView;
class QChartView;
class TransactionModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
      @brief Costruttore della main window

      Costruttore della main window
      Prepara tabella, widget grafici e button 
      Sia tabella che grafici sono posizionati in uno splitter

      @param parent QWidget parent, di default nullptr
    */
    MainWindow(QWidget *parent = nullptr);
    
    /**
      @brief Distruttore

      Distruttore
     */
    ~MainWindow();

private slots:
    /**
      @brief Slot per aggiungere una transazione
      
      Slot per aggiungere una transazione
      Apre un dialog per aggiungere una transazione
      Chiama la funzione addTransaction da transaction_model
     */
    void onAdd();


    /**
      @brief Slot per editare una transazione

      Apre un dialog per editare una transazione
      Chiama la funzione editTransaction da transaction_model
     */
    void onEdit();
    
    
    /**
      @brief Slot per delete transazioni selezionate
     
      Slot per delete transazioni selezionate
      Chiama la funzione removeTransactions da transaction_model
     */
    void onDelete();
    
    
    /**
      @brief Slot per aggiornare la view mostrando solo le transazioni filtrate
     
      Slot per aggiornare la view mostrando solo le transazioni filtrate
      Chiama la funzione setDateFilter da transaction_model
     */
    void onFilter();
    
    
    /**
      @brief Funzione che gestisce il bottone per input file CSV

      Funzione che gestisce il bottone per input file CSV
      Apre un file dialog, legge e valida il file, carica i dati nel modello
    */
    void onLoadCSV();
    
    
    /** 
      @brief Funzione che gestisce il button di salvataggio su CSV 
    
      Funzione che gestisce il button di salvataggio su CSV
      Apre un file dialog e salva su file i dati
    */
    void onSaveCSV();

    
    /**
      @brief Funzione che gestisce e aggiorna i dati visualizzati nei grafici
       
       Funzione che gestisce e aggiorna i dati visualizzati nei grafici
       Chiamata quando i dati vengono modificati, aggiorna i grafici di conseguenza
     */
    void updateGraphs();

private:
    Ui::MainWindow *ui;
    TransactionModel *model;   ///< classe TransactionModel
    QTableView *tableView;     ///< view della tabella
    QChartView *pieChartView;  ///< view del grafico a torta
    QChartView *barChartView;  ///< view del grafico a barre

}; // class MainWindow

#endif
