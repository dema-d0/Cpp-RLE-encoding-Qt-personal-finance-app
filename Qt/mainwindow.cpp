// De Meo Marco 856627 m.demeo@campus.unimib.it

/**
  @file mainwindow.cpp
  
  @brief File sorgente della main window

  File che contiene le istruzioni per la visualizzazione della main window
 */
#include "mainwindow.h"
#include "transaction_model.h"

#include <QTableView>                 // widget tabella
#include <QHeaderView>                // table header configuration
#include <QHBoxLayout>                // QHBoxLayout
#include <QVBoxLayout>                // QVBoxLayout
#include <QFileDialog>                // per i dialog su operazioni
#include <QMessageBox>
#include <QDate>                      // date x transazioni e filtering
#include <QMap>                       // categoria-spesa
#include <QSet>                       // x evitare errori duplicati nei mesi
#include <QString>

#include <QFormLayout>                // form layout in onAdd e onEdit
#include <QLineEdit>                  // QLineEdit in onAdd e onEdit
#include <QComboBox>                  // QComboBox in onAdd e onEdit
#include <QDoubleSpinBox>             // QDoubleSpinBox in onAdd e onEdit

#include <QtCharts/QChart>            // classe grafici
#include <QtCharts/QChartView>        // widget grafici
#include <QtCharts/QPieSeries>        // grafico a torta     
#include <QtCharts/QBarSeries>        // grafico a barre  
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>  // asse X - data
#include <QtCharts/QValueAxis>        // asse Y - importo


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    
    ui->setupUi(this);

    // data inizio (utile x filtro)
    ui->startDateEdit->setDate(QDate(2000, 1, 1));
    // data fine (utile x filtro)
    ui->endDateEdit->setDate(QDate(2099, 12, 31));
    
    // Setup delle views:
    // view tabella (già definita nel UI file)
    tableView = ui->tableView; 
    // oggetto TransactionModel a cui lego le view
    // il parent è la main window
    model = new TransactionModel(this);
    tableView->setModel(model);
    
    tableView->horizontalHeader()->setStretchLastSection(true); // ultima colonna, espandibile
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // colonne ridimensionabili
    tableView->setAlternatingRowColors(true); // righe alternate colorate
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // selezione per righe
    
    // view grafico a torta
    pieChartView = new QChartView(this);
    // inizializza a grafico vuoto
    pieChartView->setChart(new QChart());
    
    // view grafico istogramma
    barChartView = new QChartView(this);
    // inizializza a grafico vuoto
    barChartView->setChart(new QChart()); 

    // aggiunge i grafici al layout esistente
    // layout esistente dal file ui
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if(mainLayout) {
        // imposta la tabella con dimensioni adattive alla finestra
        tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tableView->setMinimumHeight(200);     ///< altezza minima per la tabella
        
        // Crea un container per entrambi i widget dei grafici
        QWidget *chartsWidget = new QWidget(this);
        chartsWidget->setMinimumHeight(300);  ///< altezza minima per i grafici
        chartsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        
        // HBox che contiene il container dei grafici
        // uno a fianco all'altro orizzontalmente
        QHBoxLayout *chartsLayout = new QHBoxLayout(chartsWidget);
        chartsLayout->setContentsMargins(0, 0, 0, 0); 
        
        // aggiungo il widget del grafico a torta e imposto il resizing 
        chartsLayout->addWidget(pieChartView, 1);
        pieChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        pieChartView->setMinimumSize(250, 200); ///< dimensione minima per la torta
        
        // aggiungo il widget del grafico a barre e imposto il resizing
        chartsLayout->addWidget(barChartView, 1);
        barChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        barChartView->setMinimumSize(250, 200); ///< dimensione minima per le barre
        
        // add del container cont stretch factor 2 per i grafici
        mainLayout->addWidget(chartsWidget, 2);
        
        // imposta la finestra principale per essere ridimensionabile
        this->setMinimumSize(800, 600); ///< dimensione minima della finestra
        this->resize(1000, 800);        ///< dimensione iniziale più grande
    }
    
    // buttons per I/O di CSV
    connect(ui->loadCsvButton, &QPushButton::clicked, this, &MainWindow::onLoadCSV);
    connect(ui->saveCsvButton, &QPushButton::clicked, this, &MainWindow::onSaveCSV);

    // buttons per gli slot di operazioni sulle transazioni
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::onEdit);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::onFilter);

    // lego i cambiamenti del modello ai grafici per aggiornarli DOPO che tutto è inizializzato
    // tramite signal tra il mio data model e la funzione updategraphs
    connect(model, &QAbstractItemModel::dataChanged, this, &MainWindow::updateGraphs);
    connect(model, &QAbstractItemModel::modelReset, this, &MainWindow::updateGraphs);
}

// aggiunta di transazioni
void MainWindow::onAdd() {
    // crea un dialog per inserire i dati della transazione
    QDialog dialog(this);
    // setta il titolo del dialog
    dialog.setWindowTitle("Aggiungi Transazione");
    // form layout per il dialog
    QFormLayout form(&dialog);
    // inizializza il campo data alla data di oggi
    QDateEdit dateEdit(QDate::currentDate(), &dialog);
    // campo descrizione
    QLineEdit descrizioneEdit(&dialog);
    // ComboBox per campo categoria
    QComboBox categoriaBox(&dialog);
    categoriaBox.addItems({"Casa", "Utenze", "Tempo libero e viaggi", "Alimentari e spesa", "Altre spese"});
    // SpinBox per campo importo
    QDoubleSpinBox importoSpin(&dialog);
    importoSpin.setRange(-1000000, 1000000);
    importoSpin.setDecimals(2);

    // aggiunge i widget sopra al form
    form.addRow("Data:", &dateEdit);
    form.addRow("Descrizione:", &descrizioneEdit);
    form.addRow("Categoria:", &categoriaBox);
    form.addRow("Importo:", &importoSpin);

    // buttons per OK/Cancel dell'operazione
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // se viene premuto ok
    if(dialog.exec() == QDialog::Accepted) {
        // creo una transazione con i vari campi
        Transaction t;
        t.data = dateEdit.date();
        t.descrizione = descrizioneEdit.text();
        t.categoria = categoriaBox.currentText();
        t.importo = importoSpin.value();
        // aggiungo la transazione chiamando la funzione del modello
        model->addTransaction(t);
    }
}

// edit di transazioni
void MainWindow::onEdit() {
    // indice della transazione selezionata
    QModelIndex current = tableView->currentIndex();
    // se indice non valido
    if(!current.isValid())
        return;
    int row = current.row();
    // fetch della transazione con la helper function in transaction_model
    Transaction t = model->getTransaction(row);
    // apre un dialog 
    QDialog dialog(this);
    dialog.setWindowTitle("Modifica Transazione");
    // form layout per il dialog
    QFormLayout form(&dialog);
    // inizializza i campi con i valori attuali
    // e permette la modifica
    QDateEdit dateEdit(t.data, &dialog);
    QLineEdit descrizioneEdit(t.descrizione, &dialog);
    QComboBox categoriaBox(&dialog);
    categoriaBox.addItems({"Casa", "Utenze", "Tempo libero e viaggi", "Alimentari e spesa", "Altre spese"});
    int catIndex = categoriaBox.findText(t.categoria);
    if(catIndex >= 0) categoriaBox.setCurrentIndex(catIndex);
    QDoubleSpinBox importoSpin(&dialog);
    importoSpin.setRange(-1000000, 1000000);
    importoSpin.setDecimals(2);
    importoSpin.setValue(t.importo);

    // aggiunge i widget sopra al form
    form.addRow("Data:", &dateEdit);
    form.addRow("Descrizione:", &descrizioneEdit);
    form.addRow("Categoria:", &categoriaBox);
    form.addRow("Importo:", &importoSpin);

    // buttons per OK/Cancel dell'operazione
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    // se viene premuto ok
    if(dialog.exec() == QDialog::Accepted) {
        // creo una transazione
        Transaction updated;
        // imposto i campi con i widget sopra
        updated.data = dateEdit.date();
        updated.descrizione = descrizioneEdit.text();
        updated.categoria = categoriaBox.currentText();
        updated.importo = importoSpin.value();
        // aggiorno la transazione chiamando la funzione del modello
        model->editTransaction(row, updated);
    }
}

// rimozione di transazioni
void MainWindow::onDelete() {
    // lista delle transazioni selezionate
    QModelIndexList selection = tableView->selectionModel()->selectedRows();
    if(selection.isEmpty()) {
        return;
    }
    // funzione del transaction model
    model->removeTransactions(selection);
    
}


void MainWindow::onFilter() {
    QDate from = ui->startDateEdit->date();
    QDate to = ui->endDateEdit->date();
    // funzione del transaction model
    model->setDateFilter(from, to);
}

// slot input da CSV
void MainWindow::onLoadCSV() {
    QString filename = QFileDialog::getOpenFileName(this, "Carica CSV", QString(), "CSV Files (*.csv)");
    // se filename vuoto non succede nulla
    if(filename.isEmpty()) {
        return;
    }
    QString errorMsg;
    
    // se errore nel caricamento
    if(!model->loadFromCSV(filename, errorMsg)) {
        QMessageBox::critical(this, "Errore caricamento CSV", errorMsg);
    } else {
        // update dei grafici se ha caricato il CSV
        updateGraphs();
        // forza un refresh sulla view
        tableView->viewport()->update();
        tableView->update();
    }
}

// output su un CSV
void MainWindow::onSaveCSV() {
    QString filename = QFileDialog::getSaveFileName(this, "Salva CSV", QString(), "CSV Files (*.csv)");
    // se filename vuoto non succede nulla
    if(filename.isEmpty()) return;
    QString errorMsg;
    if(!model->saveToCSV(filename, errorMsg)) {
        QMessageBox::critical(this, "Errore salvataggio CSV", errorMsg);
    }
}

// creazione e mantenimento dei grafici
void MainWindow::updateGraphs() {
    // se i widget dei grafici non sono stati inizializzati
    if(!pieChartView || !barChartView) {
        return;
    }
    // se non c'è il model
    if(!model) {
        return;
    }
    

    QChart *existingPie = pieChartView->chart();
    // clear delle series se pre esistenti 
    if(existingPie) {
        existingPie->removeAllSeries();
    }
    // clear delle series pre esistenti
    QChart *existingBar = barChartView->chart();
    if(existingBar) {
        existingBar->removeAllSeries();
    }
    // se non ci sono dati che popolano il model
    if(!model || model->rowCount() == 0) {
        return;
    }

    // grafico a torta -> distribuzione spesa x categria
    QMap<QString,double> totCategoria;
    for(int i=0; i<model->rowCount(); ++i){
        // indice importo alla quarta colonna
        QModelIndex valueIndex = model->index(i, 3);
        // indice categoria alla terza colonna
        QModelIndex catIndex = model->index(i, 2);
        // se uno degli indici non è valido
        if(!valueIndex.isValid() || !catIndex.isValid()) {
            continue;
        }
        // prende i dati nelle cellette wrappati in un QVariant
        // container generico, x flessiilità
        QVariant valueVariant = model->data(valueIndex);
        QVariant catVariant = model->data(catIndex);
        // se i dati non sono validi
        if(!valueVariant.isValid() || !catVariant.isValid()) {
            continue;
        }
        // cast importo a double    
        double value = valueVariant.toDouble();
        // il grafico a torta vuole le spese, trattate come negativi
        if(value >= 0) continue; 
        // cast categoria a QString
        QString cat = catVariant.toString();
        if(!cat.isEmpty()) {
            // accumulo le spese della categoria
            totCategoria[cat] += -value;
        }
    }

    // se ci sono i dati necessari -> creo il grafico
    if(!totCategoria.isEmpty()) {
        QPieSeries *pieSeries = new QPieSeries();
        bool hasValidData = false;
        // ciclo sulle categorie e le aggiungo al grafico
        QMap<QString,double>::iterator it;
        for(it = totCategoria.begin(); it != totCategoria.end(); ++it) {
            if(it.value() > 0) {
                // per ogni fetta devo aggiungere categoria e valor
                pieSeries->append(it.key(), it.value());
                hasValidData = true;
            }
        }
        // controllo che TUTTO sia a posto prima di usare il grafico
        if(hasValidData && pieSeries && !pieSeries->isEmpty()) {
            // ptr al grafico
            QChart *pieChart = pieChartView->chart();
            // se non esiste già un grafico, crea un nuovo grafico
            if(!pieChart) {
                pieChart = new QChart();
                pieChartView->setChart(pieChart);
            }
            if(pieChart) {
                pieChart->addSeries(pieSeries);
                pieChart->setTitle("Distribuzione spese per categoria");
                
                // imposta le label delle fette dopo averle aggiunte al grafico
                QList<QPieSlice*> slices = pieSeries->slices();
                for(int i = 0; i < slices.size(); ++i) {
                    QPieSlice *slice = slices[i];
                    if(slice) {
                        // rende visibile l'etichetta
                        slice->setLabelVisible(true);
                        // anche qui % per formattare la label
                        slice->setLabel(QString("%1: %2%")
                            // prima parte è la label
                            .arg(slice->label())
                            // seconda parte è la percentuale
                            .arg(100 * slice->percentage(), 0, 'f', 1));
                    }
                }
            }
        } else { // se non TUTTO è valido -> clear
            delete pieSeries; 
        }
    }

    ///< grafico a barre 
    // map per entrate / uscie x mese
    QMap<QString,double> entrate, uscite;
    // per i mesi uso un set perché:
    // complica un po' la vita con i cast
    // ma evita duplicati
    QSet<QString> months;
    
    // cicla sulle transazioni nel modello
    for(int i=0; i<model->rowCount(); ++i){
        // indice dell'importo alla quarta colonna
        QModelIndex valueIndex = model->index(i, 3);
        // indice della data alla prima colonna
        QModelIndex dateIndex = model->index(i, 0);
        // se uno degli indici non è valido
        if(!valueIndex.isValid() || !dateIndex.isValid()) {
            continue;
        }
        // prendo i dati dalle celle wrappati in QVariant
        // che è un container generico, x flessibilità
        QVariant valueVariant = model->data(valueIndex);
        QVariant dateVariant = model->data(dateIndex);
        // se i dati estratti non sono validi
        if(!valueVariant.isValid() || !dateVariant.isValid()) 
            continue;
        // cast importo a double
        double v = valueVariant.toDouble();
        // cast data a QString
        QString dateStr = dateVariant.toString();
        // parse della data al formato ->
        QDate d = QDate::fromString(dateStr, "dd/MM/yyyy");
        // se la data non è valida
        if(!d.isValid()) continue;
        // mi serve in questo formato ora, per gestire il "mensile"
        QString m = d.toString("yyyy-MM");
        // se ho transazioni in un mese
        if(!m.isEmpty()) {
            // lo inserisco (qui viene buono il set x duplicati)
            months.insert(m);
            // se importo positivo -> entrate
            if(v >= 0) entrate[m] += v;
            // se importo negativo -> uscite
            else uscite[m] += -v;
        }
    }

    // se ci sono dati validi tra tutti i mesi
    if(!months.isEmpty()) {
        // qua convero il QSet di mesi in QString
        QList<QString> monthList = months.values();
        std::sort(monthList.begin(), monthList.end());
        // barset per le entate
        QBarSet *ent = new QBarSet("Entrate");
        // barse per le uscite
        QBarSet *usc = new QBarSet("Uscite");
        // check di sicurezza 
        if(!ent || !usc) {
            delete ent;
            delete usc;
            return;
        }
        bool hasValidData = false;
        for(int i = 0; i < monthList.size(); ++i) {
            const QString &m = monthList[i];
            // entrate mese i (0 di default)
            double entVal = entrate.value(m, 0.0);
            // uscite mese i (0 di default)
            double uscVal = uscite.value(m, 0.0);
            ent->append(entVal);
            usc->append(uscVal);
            // se ci sono entrate o uscite -> dati validi 
            if(entVal > 0 || uscVal > 0) {
                hasValidData = true;
            }
        }
        // check anche ridondanti su barset e flag booleana
        // se è TUTTO passato allora creo il grafico
        if(hasValidData && ent && usc && (ent->count() > 0 || usc->count() > 0)) {
            QBarSeries *barSeries = new QBarSeries();
            if(barSeries) {
                // add di uscite alla barSeries
                barSeries->append(usc);
                // add di entrate alla barSeries
                barSeries->append(ent);
                // ptr al grafico
                QChart *barChart = barChartView->chart();
                // se non c'è già un grafico ne crea uno nuovo
                if(!barChart) {
                    barChart = new QChart();
                    barChartView->setChart(barChart);
                }
                if(barChart) {
                    // aggiungo le barSeries e metto il titolo al grafico
                    barChart->addSeries(barSeries);
                    barChart->setTitle("Confronto mensile Entrate/Uscite");

                    // rimuove gli assi esistenti se ce ne sono
                    QList<QAbstractAxis*> axes = barChart->axes();
                    for(int i = 0; i < axes.size(); ++i) {
                        barChart->removeAxis(axes[i]);
                    }
                    // creo gli assi nuovi
                    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
                    QValueAxis *yAxis = new QValueAxis();
                    // check su allocazione assi
                    if(xAxis && yAxis) {
                        // nomi dei mesi
                        xAxis->append(monthList);
                        // setto asse x e lo attacco centrato in basso
                        barChart->addAxis(xAxis, Qt::AlignBottom);
                        barSeries->attachAxis(xAxis);
                        // seto asse y e lo attacco centrato a sinistra
                        barChart->addAxis(yAxis, Qt::AlignLeft);
                        barSeries->attachAxis(yAxis);
                    } else {
                        delete xAxis;
                        delete yAxis;
                    }
                } else {
                    delete barSeries;
                }
            }
        } else { // clear se va storto 
            delete ent;
            delete usc;
        }
    }
}


// distruttore
MainWindow::~MainWindow() {
    delete ui;
    delete model;
}

