#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QVector>

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(QVector<int> board READ board NOTIFY boardChanged)

public:
    explicit Game(QObject *parent = nullptr);

    int score() const;
    QVector<int> board() const;

    Q_INVOKABLE void move(const QString &direction);
    Q_INVOKABLE void restart();

signals:
    void scoreChanged();
    void boardChanged();
    void gameWon();
    void gameLost();

private:
    int m_score;
    int m_board[4][4];

    void addRandomTile();
    bool moveLine(int line[4]);
    bool canMove() const;
    bool checkWin() const;
};

#endif // GAME_H
