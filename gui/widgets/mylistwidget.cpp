#include "mylistwidget.h"

MyListWidget::MyListWidget(ListState state, QWidget *parent) :
    QWidget(parent),
    m_state(state)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);
    //this->layout()->setContentsMargins(0, 5, 0, 5);

    QFrame* container = new QFrame(this);
    container->setLayout(new QVBoxLayout(container));
    container->layout()->setContentsMargins(0, 0, 0, 0);

    m_titleWidget = new QLineEdit(m_state.listName, container);
    m_titleWidget->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QObject::connect(m_titleWidget, SIGNAL(editingFinished()), this, SLOT(onTitleChanged()));
    container->layout()->addWidget(m_titleWidget);
        QFrame* mainFrame = new QFrame(container);
        mainFrame->setLayout(new QHBoxLayout(mainFrame));
        mainFrame->layout()->setContentsMargins(0, 0, 0, 0);
            QFrame* actionsFrame = new QFrame(mainFrame);
            actionsFrame->setLayout(new QVBoxLayout(actionsFrame));
            actionsFrame->layout()->setContentsMargins(0, 0, 0, 0);
            actionsFrame->layout()->setAlignment(Qt::AlignTop);
                QPushButton* addButton = new QPushButton("+", actionsFrame);
                QObject::connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addItem()));
                actionsFrame->layout()->addWidget(addButton);
                QPushButton* deleteButton = new QPushButton("-", actionsFrame);
                QObject::connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteItem()));
                actionsFrame->layout()->addWidget(deleteButton);
            mainFrame->layout()->addWidget(actionsFrame);
            QFrame* listFrame = new QFrame(mainFrame);
            listFrame->setLayout(new QVBoxLayout(listFrame));
            listFrame->layout()->setContentsMargins(0, 0, 0, 0);
                m_listWidget = new QListWidget(container);
                updateWidgets();
                listFrame->layout()->addWidget(m_listWidget);
            mainFrame->layout()->addWidget(listFrame);
        container->layout()->addWidget(mainFrame);
        this->layout()->addWidget(container);
}

ListState MyListWidget::getCurrentListState()
{
    return m_state;
}

void MyListWidget::updateWidgets()
{
    m_listWidget->clear();

    m_titleWidget->setText(m_state.listName);
    for(auto item : m_state.listItems)
    {
        QListWidgetItem* listItem = new QListWidgetItem(m_listWidget);
        MyListWidgetItem* widget = new MyListWidgetItem(item.title,
                                                        item.count,
                                                        item.selected,
                                                        m_listWidget);
        connect(widget, SIGNAL(save()), this, SLOT(onItemChanged()));
        listItem->setSizeHint(widget->minimumSizeHint());
        m_listWidget->addItem(listItem);
        m_listWidget->setItemWidget(listItem, widget);
    }
}

void MyListWidget::addItem()
{
    ItemState emptyItem;
    emptyItem.title = "";
    emptyItem.count = 1;
    emptyItem.selected = false;
    m_state.listItems.push_back(emptyItem);
    updateWidgets();
}

void MyListWidget::deleteItem()
{
    MyListWidgetItem* item = qobject_cast<MyListWidgetItem*>(m_listWidget->itemWidget(m_listWidget->currentItem()));

    ItemState content;
    content.title = item->title();
    content.count = item->count();
    content.selected = item->checked();

    m_state.listItems.removeOne(content);
    updateWidgets();
}

void MyListWidget::onItemChanged()
{
    if(m_listWidget->count() == m_state.listItems.size())
    {
        for(int i = 0; i < m_listWidget->count(); i++)
        {
            MyListWidgetItem* itemWidget =
                    qobject_cast<MyListWidgetItem*>(m_listWidget->itemWidget(m_listWidget->item(i)));
            disconnect(itemWidget, SIGNAL(save()), this, SLOT(onItemChanged()));

            ItemState item;
            item.title = itemWidget->title();
            item.count = itemWidget->count();
            item.selected = itemWidget->checked();

            m_state.listItems[i] = item;
        }
        //updateList();
    }
}

void MyListWidget::onTitleChanged()
{
    m_state.listName = m_titleWidget->text();
}
