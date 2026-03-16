#include "game.h"
#include <QRandomGenerator>
#include <algorithm>

// ── Constructeur ──────────────────────────────────────────────
Game::Game(QObject *parent) : QObject(parent)
{
    restart();
}

// ── Getters ───────────────────────────────────────────────────
int Game::score() const
{
    return m_score;
}

QVector<int> Game::board() const
{
    QVector<int> flat;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            flat.append(m_board[r][c]);
    return flat;
}

// ── Restart ───────────────────────────────────────────────────
void Game::restart()
{
    m_score = 0;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            m_board[r][c] = 0;

    addRandomTile();
    addRandomTile();

    emit scoreChanged();
    emit boardChanged();
}

// ── Ajout d'une tuile aléatoire ───────────────────────────────
void Game::addRandomTile()
{
    QVector<QPair<int,int>> empty;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (m_board[r][c] == 0)
                empty.append({r, c});

    if (empty.isEmpty()) return;

    int idx = QRandomGenerator::global()->bounded(empty.size());
    int value = (QRandomGenerator::global()->bounded(10) < 9) ? 2 : 4;
    m_board[empty[idx].first][empty[idx].second] = value;
}

// ── Déplacement d'une ligne vers la gauche ────────────────────
bool Game::moveLine(int line[4])
{
    bool changed = false;

    // Étape 1 : tasser les tuiles (supprimer les zéros)
    int temp[4] = {0, 0, 0, 0};
    int pos = 0;
    for (int i = 0; i < 4; i++)
        if (line[i] != 0)
            temp[pos++] = line[i];

    // Étape 2 : fusionner les tuiles identiques adjacentes
    for (int i = 0; i < 3; i++) {
        if (temp[i] != 0 && temp[i] == temp[i+1]) {
            temp[i] *= 2;
            m_score += temp[i];
            temp[i+1] = 0;
        }
    }

    // Étape 3 : retasser après fusion
    int result[4] = {0, 0, 0, 0};
    pos = 0;
    for (int i = 0; i < 4; i++)
        if (temp[i] != 0)
            result[pos++] = temp[i];

    // Vérifier si la ligne a changé
    for (int i = 0; i < 4; i++) {
        if (line[i] != result[i]) changed = true;
        line[i] = result[i];
    }

    return changed;
}

// ── Déplacements dans les 4 directions ───────────────────────
void Game::move(const QString &direction)
{
    bool changed = false;

    if (direction == "left") {
        for (int r = 0; r < 4; r++)
            if (moveLine(m_board[r])) changed = true;
    }
    else if (direction == "right") {
        for (int r = 0; r < 4; r++) {
            int line[4] = { m_board[r][3], m_board[r][2], m_board[r][1], m_board[r][0] };
            if (moveLine(line)) {
                changed = true;
                m_board[r][3] = line[0]; m_board[r][2] = line[1];
                m_board[r][1] = line[2]; m_board[r][0] = line[3];
            }
        }
    }
    else if (direction == "up") {
        for (int c = 0; c < 4; c++) {
            int line[4] = { m_board[0][c], m_board[1][c], m_board[2][c], m_board[3][c] };
            if (moveLine(line)) {
                changed = true;
                m_board[0][c] = line[0]; m_board[1][c] = line[1];
                m_board[2][c] = line[2]; m_board[3][c] = line[3];
            }
        }
    }
    else if (direction == "down") {
        for (int c = 0; c < 4; c++) {
            int line[4] = { m_board[3][c], m_board[2][c], m_board[1][c], m_board[0][c] };
            if (moveLine(line)) {
                changed = true;
                m_board[3][c] = line[0]; m_board[2][c] = line[1];
                m_board[1][c] = line[2]; m_board[0][c] = line[3];
            }
        }
    }

    if (changed) {
        addRandomTile();
        emit boardChanged();
        emit scoreChanged();
    }

    if (checkWin())  emit gameWon();
    if (!canMove())  emit gameLost();
}

// ── Vérification victoire ─────────────────────────────────────
bool Game::checkWin() const
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (m_board[r][c] == 2048) return true;
    return false;
}

// ── Vérification défaite ──────────────────────────────────────
bool Game::canMove() const
{
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++) {
            if (m_board[r][c] == 0) return true;
            if (c < 3 && m_board[r][c] == m_board[r][c+1]) return true;
            if (r < 3 && m_board[r][c] == m_board[r+1][c]) return true;
        }
    return false;
}
