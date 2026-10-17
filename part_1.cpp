#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

const int MAX_TASKS = 50;
string taskName[MAX_TASKS];
int deadline[MAX_TASKS];
int importance[MAX_TASKS];
int difficulty[MAX_TASKS];
int urgency[MAX_TASKS];
int priority[MAX_TASKS];
int skippedDays[MAX_TASKS];
bool completedTask[MAX_TASKS];

int taskCount = 0;

void clearScreen()
{
    system("cls");
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void calculatePriority(int index)
{
    int score = 0;

    if (deadline[index] == 1)
    {
        score += 5;
    }
    else if (deadline[index] == 2)
    {
        score = score + 4;
    }
    else if (deadline[index] == 3)
    {
        score = score + 2;
    }
    else
    {
        score = score + 1;
    }

    if (importance[index] == 3)
    {
        score = score + 5;
    }
    else if (importance[index] == 2)
    {
        score = score + 3;
    }
    else
    {
        score = score + 1;
    }

    if (difficulty[index] == 3)
    {
        score = score + 3;
    }
    else if (difficulty[index] == 2)
    {
        score = score + 2;
    }
    else
    {
        score = score + 1;
    }

    if (urgency[index] == 3)
    {
        score = score + 5;
    }
    else if (urgency[index] == 2)
    {
        score = score + 3;
    }
    else
    {
        score = score + 1;
    } 

    if (skippedDays[index] == 1)
    {
        score = score + 1;
    }
    else if (skippedDays[index] == 2)
    {
        score = score + 2;
    }
    else if (skippedDays[index] >= 3)
    {
        score = score + 3;
    }

    priority[index] = score;
}

void addTask()
{
    clearScreen();

    if (taskCount >= MAX_TASKS)
    {
        cout << "Task limit reached.\n";
        pauseScreen();
        return;
    }

    cin.ignore();

    cout << "========== ADD TASK ==========\n\n";

    cout << "Enter task name: ";
    getline(cin, taskName[taskCount]);

    cout << "\nDeadline:\n";
    cout << "1. Due today\n";
    cout << "2. Due tomorrow\n";
    cout << "3. Due within a week\n";
    cout << "4. More than a week\n";
    cout << "Enter choice: ";
    cin >> deadline[taskCount];

    cout << "\nImportance:\n";
    cout << "1. Low\n";
    cout << "2. Medium\n";
    cout << "3. High\n";
    cout << "Enter choice: ";
    cin >> importance[taskCount];

    cout << "\nDifficulty:\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Enter choice: ";
    cin >> difficulty[taskCount];

    cout << "\nUrgency:\n";
    cout << "1. Low\n";
    cout << "2. Medium\n";
    cout << "3. High\n";
    cout << "Enter choice: ";
    cin >> urgency[taskCount];

    skippedDays[taskCount] = 0;
    completedTask[taskCount] = false;

    calculatePriority(taskCount);

    taskCount++;

    cout << "\nTask added successfully!\n";

    pauseScreen();
    clearScreen();
}

void displayTasks()
{
    clearScreen();

    cout << "========== TASK LIST ==========\n\n";

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        cout << "Task #" << i + 1 << "\n";
        cout << "Name       : " << taskName[i] << "\n";
        cout << "Priority   : " << priority[i] << "\n";
        cout << "Skipped    : " << skippedDays[i] << " day(s)\n";

        if (completedTask[i] == true)
        {
            cout << "Status     : Completed\n";
        }
        else
        {
            cout << "Status     : Pending\n";
        }

        cout << "-------------------------------\n";
    }

    pauseScreen();
    clearScreen();
}

void taskSummary()
{
    clearScreen();

    int pending = 0;
    int completed = 0;
    int highestPriority = 0;

    cout << "========== TASK SUMMARY ==========\n\n";

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (completedTask[i] == true)
        {
            completed++;
        }
        else
        {
            pending++;
        }

        if (priority[i] > highestPriority)
        {
            highestPriority = priority[i];
        }
    }

    cout << "Total Tasks      : " << taskCount << "\n";
    cout << "Pending Tasks    : " << pending << "\n";
    cout << "Completed Tasks  : " << completed << "\n";
    cout << "Highest Priority : " << highestPriority << "\n";

    pauseScreen();
    clearScreen();
}

void sortTasks()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available to sort.\n";
        pauseScreen();
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        calculatePriority(i);
    }

    for (int i = 0; i < taskCount - 1; i++)
    {
        for (int j = 0; j < taskCount - i - 1; j++)
        {
            if (priority[j] < priority[j + 1])
            {
                int tempPriority = priority[j];
                priority[j] = priority[j + 1];
                priority[j + 1] = tempPriority;

                string tempName = taskName[j];
                taskName[j] = taskName[j + 1];
                taskName[j + 1] = tempName;

                int tempDeadline = deadline[j];
                deadline[j] = deadline[j + 1];
                deadline[j + 1] = tempDeadline;

                int tempImportance = importance[j];
                importance[j] = importance[j + 1];
                importance[j + 1] = tempImportance;

                int tempDifficulty = difficulty[j];
                difficulty[j] = difficulty[j + 1];
                difficulty[j + 1] = tempDifficulty;

                int tempUrgency = urgency[j];
                urgency[j] = urgency[j + 1];
                urgency[j + 1] = tempUrgency;

                int tempSkipped = skippedDays[j];
                skippedDays[j] = skippedDays[j + 1];
                skippedDays[j + 1] = tempSkipped;

                bool tempCompleted = completedTask[j];
                completedTask[j] = completedTask[j + 1];
                completedTask[j + 1] = tempCompleted;
            }
        }
    }

    cout << "Tasks have been organized by priority.\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << taskName[i] << " | Priority: " << priority[i];

        if (completedTask[i] == true)
        {
            cout << " | Completed";
        }
        else
        {
            cout << " | Pending";
        }

        cout << "\n";
    }

    pauseScreen();
    clearScreen();
}

void completeTask()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    cout << "========== COMPLETE TASK ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << taskName[i];

        if (completedTask[i] == true)
        {
            cout << " [Completed]";
        }

        cout << "\n";
    }

    int choice;

    cout << "\nEnter task number to complete: ";
    cin >> choice;

    if (choice >= 1 && choice <= taskCount)
    {
        if (completedTask[choice - 1] == false)
        {
            completedTask[choice - 1] = true;
            cout << "\nTask marked as completed!\n";
        }
        else
        {
            cout << "\nThis task is already completed.\n";
        }
    }
    else
    {
        cout << "\nInvalid task number.\n";
    }

    pauseScreen();
    clearScreen();
}

void updateSkippedDays()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    cout << "========== SKIPPED TASKS ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << taskName[i]
             << " | Skipped: " << skippedDays[i] << " day(s)\n";
    }

    int choice;

    cout << "\nEnter task number that was skipped: ";
    cin >> choice;

    if (choice >= 1 && choice <= taskCount)
    {
        if (completedTask[choice - 1] == false)
        {
            skippedDays[choice - 1]++;

            calculatePriority(choice - 1);

            cout << "\nTask skipped for one more day.\n";
            cout << "New priority: " << priority[choice - 1] << "\n";
        }
        else
        {
            cout << "\nThis task is already completed.\n";
        }
    }
    else
    {
        cout << "\nInvalid task number.\n";
    }

    pauseScreen();
    clearScreen();
}

void deleteTask()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    cout << "========== DELETE TASK ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << taskName[i] << "\n";
    }

    int choice;

    cout << "\nEnter task number to delete: ";
    cin >> choice;

    if (choice >= 1 && choice <= taskCount)
    {
        for (int i = choice - 1; i < taskCount - 1; i++)
        {
            taskName[i] = taskName[i + 1];
            deadline[i] = deadline[i + 1];
            importance[i] = importance[i + 1];
            difficulty[i] = difficulty[i + 1];
            urgency[i] = urgency[i + 1];
            priority[i] = priority[i + 1];
            skippedDays[i] = skippedDays[i + 1];
            completedTask[i] = completedTask[i + 1];
        }

        taskCount--;

        cout << "\nTask deleted successfully!\n";
    }
    else
    {
        cout << "\nInvalid task number.\n";
    }

    pauseScreen();
    clearScreen();
}

void editTask()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    cout << "========== EDIT TASK ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        cout << i + 1 << ". " << taskName[i] << "\n";
    }

    int choice;

    cout << "\nEnter task number to edit: ";
    cin >> choice;

    if (choice >= 1 && choice <= taskCount)
    {
        cout << "\n1. Edit Name\n";
        cout << "2. Edit Deadline\n";
        cout << "3. Edit Importance\n";
        cout << "4. Edit Difficulty\n";
        cout << "5. Edit Urgency\n";
        cout << "6. Edit Status\n";
        cout << "Enter your choice: ";

        int option;
        cin >> option;

        if (option == 1)
        {
            cin.ignore();
            cout << "Enter new task name: ";
            getline(cin, taskName[choice - 1]);
        }
        else if (option == 2)
        {
            cout << "\nDeadline:\n";
            cout << "1. Due today\n";
            cout << "2. Due tomorrow\n";
            cout << "3. Due within a week\n";
            cout << "4. More than a week\n";
            cout << "Enter choice: ";
            cin >> deadline[choice - 1];
        }
        else if (option == 3)
        {
            cout << "\nImportance:\n";
            cout << "1. Low\n";
            cout << "2. Medium\n";
            cout << "3. High\n";
            cout << "Enter choice: ";
            cin >> importance[choice - 1];
        }
        else if (option == 4)
        {
            cout << "\nDifficulty:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Hard\n";
            cout << "Enter choice: ";
            cin >> difficulty[choice - 1];
        }
        else if (option == 5)
        {
            cout << "\nUrgency:\n";
            cout << "1. Low\n";
            cout << "2. Medium\n";
            cout << "3. High\n";
            cout << "Enter choice: ";
            cin >> urgency[choice - 1];
        }
        else if (option == 6)
        {
            cout << "\n1. Mark as Pending\n";
            cout << "2. Mark as Completed\n";
            cout << "Enter choice: ";

            int statusChoice;
            cin >> statusChoice;

            if (statusChoice == 1)
            {
                completedTask[choice - 1] = false;
            }
            else if (statusChoice == 2)
            {
                completedTask[choice - 1] = true;
            }
        }

        calculatePriority(choice - 1);

        cout << "\nTask updated successfully!\n";
    }
    else
    {
        cout << "\nInvalid task number.\n";
    }

    pauseScreen();
    clearScreen();
}

void searchTask()
{
    clearScreen();

    if (taskCount == 0)
    {
        cout << "No tasks available.\n";
        pauseScreen();
        return;
    }

    cout << "========== SEARCH TASK ==========\n\n";
    cout << "Enter task name to search: ";
    cin.ignore();
    string searchName;
    getline(cin, searchName);

    bool found = false;

    for (int i = 0; i < taskCount; i++)
    {
        if (taskName[i] == searchName)
        {
            cout << "\nTask Found!\n";
            cout << "Task #: " << i + 1 << "\n";
            cout << "Name       : " << taskName[i] << "\n";
            cout << "Priority   : " << priority[i] << "\n";
            cout << "Skipped    : " << skippedDays[i] << " day(s)\n";

            if (completedTask[i] == true)
            {
                cout << "Status     : Completed\n";
            }
            else
            {
                cout << "Status     : Pending\n";
            }

            found = true;
            break;
        }
    }

    if (found == false)
    {
        cout << "\nTask not found.\n";
    }

    pauseScreen();
    clearScreen();
}

void viewPendingTasks()
{
    clearScreen();

    bool hasPending = false;

    cout << "========== PENDING TASKS ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        if (completedTask[i] == false)
        {
            cout << i + 1 << ". " << taskName[i] << " | Priority: " << priority[i] << "\n";
            hasPending = true;
        }
    }

    if (hasPending == false)
    {
        cout << "No pending tasks.\n";
    }

    pauseScreen();
    clearScreen();
}

void viewCompletedTasks()
{
    clearScreen();

    bool hasCompleted = false;

    cout << "========== COMPLETED TASKS ==========\n\n";

    for (int i = 0; i < taskCount; i++)
    {
        if (completedTask[i] == true)
        {
            cout << i + 1 << ". " << taskName[i] << " | Priority: " << priority[i] << "\n";
            hasCompleted = true;
        }
    }

    if (hasCompleted == false)
    {
        cout << "No completed tasks.\n";
    }

    pauseScreen();
    clearScreen();
}

void displayMenu()
{
    cout << "====================================\n";
    cout << "              TASKMIND              \n";
    cout << "     Smart Task Management System    \n";
    cout << "====================================\n\n";

    cout << "1. Add Task\n";
    cout << "2. View Tasks\n";
    cout << "3. Auto-Organize by Priority\n";
    cout << "4. Complete Task\n";
    cout << "5. Update Skipped Days\n";
    cout << "6. Delete Task\n";
    cout << "7. Edit Task\n";
    cout << "8. Search Task\n";
    cout << "9. Task Summary\n";
    cout << "10. Pending Tasks\n";
    cout << "11. Completed Tasks\n";
    cout << "12. Exit\n\n";
}

int main()
{
    int choice = 0;

    do
    {
        clearScreen();
        displayMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            addTask();
        }
        else if (choice == 2)
        {
            displayTasks();
        }
        else if (choice == 3)
        {
            sortTasks();
        }
        else if (choice == 4)
        {
            completeTask();
        }
        else if (choice == 5)
        {
            updateSkippedDays();
        }
        else if (choice == 6)
        {
            deleteTask();
        }
        else if (choice == 7)
        {
            editTask();
        }
        else if (choice == 8)
        {
            searchTask();
        }
        else if (choice == 9)
        {
            taskSummary();
        }
        else if (choice == 10)
        {
            viewPendingTasks();
        }
        else if (choice == 11)
        {
            viewCompletedTasks();
        }
        else if (choice == 12)
        {
            clearScreen();

            cout << "====================================\n";
            cout << "       Thank you for using          \n";
            cout << "             TASKMIND               \n";
            cout << "====================================\n";
        }
        else
        {
            cout << "\nInvalid choice. Please try again.\n";
            pauseScreen();
        }

    } while (choice != 12);

    return 0;
}