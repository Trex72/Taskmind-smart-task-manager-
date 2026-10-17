// ============================================================
// TaskMind - Part 2
// Beginner C++ project using structures, a class, and objects
// New features: real calendar dates and file saving
// ============================================================

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

const int MAX_TASKS = 50;
const string DATA_FILE = "taskmind_data.txt";

// A small structure groups the parts that belong to one date.
struct Date
{
    int year;
    int month;
    int day;
};

// One Task object stores all information for one task.
struct Task
{
    string name;
    Date dueDate;
    int importance;
    int difficulty;
    int urgency;
    int skippedDays;
    bool completed;
};

// ---------- Basic input helpers ----------
int getNumber(const string& prompt, int low, int high)
{
    int value;

    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= low && value <= high)
        {
            cin.ignore(1000, '\n');
            return value;
        }

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number from " << low << " to " << high << ".\n";
    }
}

string getTaskName()
{
    string name;
    cout << "Task name: ";
    getline(cin, name);

    while (name.empty())
    {
        cout << "Task name cannot be empty. Try again: ";
        getline(cin, name);
    }
    return name;
}

bool isLeapYear(int year)
{
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    return year % 4 == 0;
}

int daysInMonth(int month, int year)
{
    if (month == 2)
    {
        if (isLeapYear(year)) return 29;
        return 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

bool isValidDate(Date date)
{
    if (date.year < 1900 || date.year > 9999) return false;
    if (date.month < 1 || date.month > 12) return false;
    if (date.day < 1 || date.day > daysInMonth(date.month, date.year)) return false;
    return true;
}

Date getDate(const string& prompt)
{
    Date date;
    bool valid = false;

    while (!valid)
    {
        cout << prompt << "\n";
        date.year = getNumber("  Year (1900-9999): ", 1900, 9999);
        date.month = getNumber("  Month (1-12): ", 1, 12);
        date.day = getNumber("  Day: ", 1, 31);
        valid = isValidDate(date);

        if (!valid)
            cout << "That date does not exist. Please enter it again.\n";
    }
    return date;
}

Date getToday()
{
    time_t now = time(0);
    tm* localTime = localtime(&now);

    Date today;
    today.year = localTime->tm_year + 1900;
    today.month = localTime->tm_mon + 1;
    today.day = localTime->tm_mday;
    return today;
}

// Changes a date into a number of days so two dates can be compared.
long dateNumber(Date date)
{
    long total = 0;

    for (int year = 1; year < date.year; year++)
    {
        if (isLeapYear(year)) total += 366;
        else total += 365;
    }

    for (int month = 1; month < date.month; month++)
        total += daysInMonth(month, date.year);

    total += date.day;
    return total;
}

int daysUntil(Date dueDate)
{
    Date today = getToday();
    return static_cast<int>(dateNumber(dueDate) - dateNumber(today));
}

void printDate(Date date)
{
    cout << date.year << "/";
    if (date.month < 10) cout << "0";
    cout << date.month << "/";
    if (date.day < 10) cout << "0";
    cout << date.day;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

// TaskManager is the back-end object. It stores tasks and performs the work.
class TaskManager
{
private:
    Task tasks[MAX_TASKS];
    int taskCount;

    int calculatePriority(int index)
    {
        int remainingDays = daysUntil(tasks[index].dueDate);
        int deadlinePoints;

        if (remainingDays <= 0) deadlinePoints = 5;
        else if (remainingDays == 1) deadlinePoints = 4;
        else if (remainingDays <= 3) deadlinePoints = 3;
        else if (remainingDays <= 7) deadlinePoints = 2;
        else deadlinePoints = 1;

        int score = deadlinePoints + tasks[index].importance
                  + tasks[index].difficulty + tasks[index].urgency
                  + tasks[index].skippedDays;
        return score;
    }

    void printTask(int index, int number)
    {
        int remainingDays = daysUntil(tasks[index].dueDate);
        cout << number << ". " << tasks[index].name << "\n";
        cout << "   Due: ";
        printDate(tasks[index].dueDate);
        cout << "   Days left: " << remainingDays;
        if (remainingDays < 0) cout << " (overdue)";
        cout << "\n   Priority: " << calculatePriority(index)
             << "   Skipped days: " << tasks[index].skippedDays
             << "   Status: ";
        if (tasks[index].completed) cout << "Completed\n";
        else cout << "Pending\n";
    }

public:
    // Creating this object loads saved tasks from the file.
    TaskManager()
    {
        taskCount = 0;
        loadTasks();
    }

    void loadTasks()
    {
        ifstream inputFile(DATA_FILE.c_str());
        if (!inputFile) return;

        int savedCount;
        inputFile >> savedCount;
        inputFile.ignore(1000, '\n');

        if (!inputFile || savedCount < 0 || savedCount > MAX_TASKS)
        {
            taskCount = 0;
            return;
        }

        taskCount = 0;
        for (int i = 0; i < savedCount; i++)
        {
            Task loadedTask;
            getline(inputFile, loadedTask.name);
            inputFile >> loadedTask.dueDate.year
                      >> loadedTask.dueDate.month
                      >> loadedTask.dueDate.day
                      >> loadedTask.importance
                      >> loadedTask.difficulty
                      >> loadedTask.urgency
                      >> loadedTask.skippedDays
                      >> loadedTask.completed;
            inputFile.ignore(1000, '\n');

            if (!inputFile || !isValidDate(loadedTask.dueDate))
                break;

            tasks[taskCount] = loadedTask;
            taskCount++;
        }
    }

    void saveTasks()
    {
        ofstream outputFile(DATA_FILE.c_str());
        if (!outputFile)
        {
            cout << "Could not open the task data file for saving.\n";
            return;
        }

        outputFile << taskCount << "\n";
        for (int i = 0; i < taskCount; i++)
        {
            outputFile << tasks[i].name << "\n";
            outputFile << tasks[i].dueDate.year << " "
                       << tasks[i].dueDate.month << " "
                       << tasks[i].dueDate.day << " "
                       << tasks[i].importance << " "
                       << tasks[i].difficulty << " "
                       << tasks[i].urgency << " "
                       << tasks[i].skippedDays << " "
                       << tasks[i].completed << "\n";
        }
    }

    void addTask()
    {
        if (taskCount >= MAX_TASKS)
        {
            cout << "The task list is full.\n";
            return;
        }

        Task newTask;
        newTask.name = getTaskName();
        newTask.dueDate = getDate("Enter the due date");
        newTask.importance = getNumber("Importance (1 low to 5 high): ", 1, 5);
        newTask.difficulty = getNumber("Difficulty (1 easy to 5 hard): ", 1, 5);
        newTask.urgency = getNumber("Urgency (1 low to 5 high): ", 1, 5);
        newTask.skippedDays = 0;
        newTask.completed = false;

        tasks[taskCount] = newTask;
        taskCount++;
        saveTasks();

        cout << "Task added and saved.\n";
    }

    void displayTasks(bool pendingOnly = false, bool completedOnly = false)
    {
        bool found = false;
        cout << "\nTASK LIST\n----------------------------------------\n";

        for (int i = 0; i < taskCount; i++)
        {
            if (pendingOnly && tasks[i].completed) continue;
            if (completedOnly && !tasks[i].completed) continue;
            printTask(i, i + 1);
            cout << "----------------------------------------\n";
            found = true;
        }

        if (!found) cout << "No matching tasks were found.\n";
    }

    void sortTasks()
    {
        for (int pass = 0; pass < taskCount - 1; pass++)
        {
            for (int i = 0; i < taskCount - 1 - pass; i++)
            {
                if (calculatePriority(i) < calculatePriority(i + 1))
                {
                    Task temporaryTask = tasks[i];
                    tasks[i] = tasks[i + 1];
                    tasks[i + 1] = temporaryTask;
                }
            }
        }
        saveTasks();
        cout << "Tasks organized from highest priority to lowest.\n";
        displayTasks();
    }

    void completeTask()
    {
        displayTasks();
        if (taskCount == 0) return;

        int number = getNumber("Enter task number to complete (0 to cancel): ", 0, taskCount);
        if (number == 0) return;

        tasks[number - 1].completed = true;
        saveTasks();
        cout << "Task marked completed and saved.\n";
    }

    void updateSkippedDays()
    {
        displayTasks(true, false);
        if (taskCount == 0) return;

        int number = getNumber("Enter pending task number (0 to cancel): ", 0, taskCount);
        if (number == 0) return;
        if (tasks[number - 1].completed)
        {
            cout << "That task is already completed.\n";
            return;
        }

        tasks[number - 1].skippedDays++;
        saveTasks();
        cout << "Skipped days increased. Priority will now be higher.\n";
    }

    void deleteTask()
    {
        displayTasks();
        if (taskCount == 0) return;

        int number = getNumber("Enter task number to delete (0 to cancel): ", 0, taskCount);
        if (number == 0) return;

        for (int i = number - 1; i < taskCount - 1; i++)
            tasks[i] = tasks[i + 1];

        taskCount--;
        saveTasks();
        cout << "Task deleted and file updated.\n";
    }

    void editTask()
    {
        displayTasks();
        if (taskCount == 0) return;

        int number = getNumber("Enter task number to edit (0 to cancel): ", 0, taskCount);
        if (number == 0) return;

        Task& selectedTask = tasks[number - 1];
        cout << "1. Change name\n2. Change due date\n3. Change importance\n";
        cout << "4. Change difficulty\n5. Change urgency\n6. Change status\n";
        cout << "0. Cancel\n";
        int choice = getNumber("Choose what to change: ", 0, 6);

        if (choice == 1) selectedTask.name = getTaskName();
        else if (choice == 2) selectedTask.dueDate = getDate("Enter the new due date");
        else if (choice == 3) selectedTask.importance = getNumber("Importance (1-5): ", 1, 5);
        else if (choice == 4) selectedTask.difficulty = getNumber("Difficulty (1-5): ", 1, 5);
        else if (choice == 5) selectedTask.urgency = getNumber("Urgency (1-5): ", 1, 5);
        else if (choice == 6)
        {
            cout << "1. Pending\n2. Completed\n";
            selectedTask.completed = getNumber("Choose status: ", 1, 2) == 2;
        }

        if (choice != 0)
        {
            saveTasks();
            cout << "Task updated and saved.\n";
        }
    }

    void searchTask()
    {
        string searchName;
        cout << "Enter the exact task name: ";
        getline(cin, searchName);

        for (int i = 0; i < taskCount; i++)
        {
            if (tasks[i].name == searchName)
            {
                printTask(i, i + 1);
                return;
            }
        }
        cout << "Task not found.\n";
    }

    void taskSummary()
    {
        int pendingCount = 0;
        int completedCount = 0;
        int highestIndex = -1;

        for (int i = 0; i < taskCount; i++)
        {
            if (tasks[i].completed) completedCount++;
            else pendingCount++;

            if (highestIndex == -1 || calculatePriority(i) > calculatePriority(highestIndex))
                highestIndex = i;
        }

        cout << "\nTASK SUMMARY\n";
        cout << "Total tasks: " << taskCount << "\n";
        cout << "Pending tasks: " << pendingCount << "\n";
        cout << "Completed tasks: " << completedCount << "\n";
        if (highestIndex != -1)
        {
            cout << "Highest priority task: " << tasks[highestIndex].name
                 << " (score " << calculatePriority(highestIndex) << ")\n";
        }
    }
};

// The menu is the simple front end. It gets choices and asks the object to work.
void displayMenu()
{
    Date today = getToday();
    cout << "============================================\n";
    cout << "                  TASKMIND                  \n";
    cout << "       Simple Smart Task Manager            \n";
    cout << "============================================\n";
    cout << "Today: ";
    printDate(today);
    cout << "\n\n";
    cout << "1. Add task\n";
    cout << "2. View all tasks\n";
    cout << "3. Auto-organize by priority\n";
    cout << "4. Complete task\n";
    cout << "5. Update skipped days\n";
    cout << "6. Delete task\n";
    cout << "7. Edit task\n";
    cout << "8. Search task\n";
    cout << "9. Task summary\n";
    cout << "10. View pending tasks\n";
    cout << "11. View completed tasks\n";
    cout << "12. Exit\n\n";
}

int main()
{
    TaskManager taskManager; // This object loads the saved tasks when the program starts.
    int choice = 0;

    do
    {
        clearScreen();
        displayMenu();
        choice = getNumber("Enter your choice: ", 1, 12);
        clearScreen();

        if (choice == 1) taskManager.addTask();
        else if (choice == 2) taskManager.displayTasks();
        else if (choice == 3) taskManager.sortTasks();
        else if (choice == 4) taskManager.completeTask();
        else if (choice == 5) taskManager.updateSkippedDays();
        else if (choice == 6) taskManager.deleteTask();
        else if (choice == 7) taskManager.editTask();
        else if (choice == 8) taskManager.searchTask();
        else if (choice == 9) taskManager.taskSummary();
        else if (choice == 10) taskManager.displayTasks(true, false);
        else if (choice == 11) taskManager.displayTasks(false, true);
        else if (choice == 12) cout << "Thank you for using TaskMind.\n";

        if (choice != 12) pauseScreen();
    }
    while (choice != 12);

    return 0;
}
