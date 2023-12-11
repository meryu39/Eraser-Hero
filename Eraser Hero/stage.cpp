#include "Stage.h"

int currentStage;

Stage::Stage(SDL_Renderer* renderer)
{
    stageRenderer = renderer;
    srand(static_cast<unsigned>(time(nullptr)));
}

Stage::~Stage()
{
}

void Stage::spawnRandomEnemy()
{
    const std::vector<std::pair<int, int>> spawnPoints = {
        {635, 0},
        {635, 715},
        {1275, 355},
        {5, 355}
    };

    const auto randomIndex = rand() % spawnPoints.size();
    const auto spawnPoint = spawnPoints[randomIndex];

    // ���� �������� ��ȣ�� ���� enemyType�� ������ ����
    int maxEnemyType = 0; // �������� 1�� ��
    if (currentStage >= 2) maxEnemyType = 1; // �������� 2�� ��
    if (currentStage >= 3) maxEnemyType = 2; // �������� 3�� ��
    if (currentStage >= 4) maxEnemyType = 3; // �������� 4�� ��

    const auto enemyType = rand() % (maxEnemyType + 1);

    switch (enemyType)
    {
    case 0:
        new Pencil(stageRenderer, spawnPoint.first, spawnPoint.second);
        break;
    case 1:
        new Sharp(stageRenderer, spawnPoint.first, spawnPoint.second);
        break;
    case 2:
        new Fountain(stageRenderer, spawnPoint.first, spawnPoint.second);
        break;
    case 3:
        new Compass(stageRenderer, spawnPoint.first, spawnPoint.second);
        break;
    default:
        break;
    }
}
