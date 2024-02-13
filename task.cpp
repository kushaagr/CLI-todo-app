
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
// #include <ios>
#include <iomanip>
// #include <forward_list>
#include <cstring>
#include <list>
#include <unordered_map>
#include <vector>
#include <array>

// #include <Windows.h>

using std::cout;

#define COMMAND     g_argv[1] 
#define PRIORITY    g_argv[2]
#define INDEX       g_argv[2]
#define TASK        g_argv[3]
#define Nl          u8"\n"
// #define NL          "\r\n"
// #define NL ""

struct Task {
    int priority;
    // std::string value;
    std::string value;

    Task(int p, std::string v) {
        priority = p;
        value = v;
    }

    bool operator()(const Task& a, const Task& b) {
      return a.priority < b.priority;
    }
};

typedef std::vector<std::string> strings;

int g_argc;
strings g_argv;
std::string g_appname;
// char * g_taskfile;
// char * g_donefile;

enum ERRORS { 
    SUCCESS = EXIT_SUCCESS,
    NEGATIVE_PRIO,
    MISSING_ADD_ARGS,
    MISSING_DEL_ARGS,
    MISSING_DON_ARGS,
    NO_TASK_DEL,
    NO_TASK_DONE
};


class ITaskList {
protected:
    virtual Task getPendingTask(int index) =0;
    // virtual void* getPendingTask(int index) =0;
    virtual Task getDoneTask(int index) =0;
    // virtual void* getDoneTask(int index) =0;
    virtual int getPendingListSize() =0;
    virtual int getDoneListSize() =0;
    virtual bool addToPendingTasks(const Task& t) =0;
    virtual bool addToDoneTasks(const Task& t) =0;
    virtual bool removePendingTask(int index) =0;
    virtual bool removeDoneTask(int index) =0;
    virtual bool loadDataP() =0;
    virtual bool loadDataD() =0;
    virtual bool unloadDataP() =0;
    virtual bool unloadDataD() =0;

};
    
// bool compareTaskByPriority(const Task& a, const Task& b) {
//   return a.priority < b.priority;
// }

struct Itercache {
    std::list<Task>::iterator it;
    int index;
    Itercache(std::list<Task>::iterator it, int index) {
        this->it = it;
        this->index = index;
    }
};


class PriorityTaskList : public ITaskList {
private:
    std::list<Task> pendingTasks;
    std::list<Task> doneTasks;
    // std::string taskfile;
    // std::string donefile;
    const std::string taskfile;
    const std::string donefile;
    Itercache pic, dic;

public:
    PriorityTaskList(const std::string &filename1,
     const std::string &filename2) 
        : taskfile(filename1),
        donefile(filename2),
        pic(pendingTasks.begin(), 1),
        dic(doneTasks.begin(), 1)
        {}

    // ~PriorityTaskList() {
    //     pendingTasks.clear();
    //     doneTasks.clear();
    // }

    inline int getPendingListSize() {
        // return std::distance(pendingTasks.begin(), pendingTasks.end());
        return pendingTasks.size();
    }

    inline int getDoneListSize() {
        // return std::distance(doneTasks.begin(), doneTasks.end());
        return doneTasks.size();
    }

    // std::list<Task>::iterator getPendingTask(int index) {
    Task getPendingTask(int index) {
        int listsz = getPendingListSize();
        if( ((index) > listsz) || index < 1 ) 
            // return nullptr;
            return Task(-1, "");
        

        cout << listsz << Nl;
        
        auto it = pendingTasks.begin();
        int i = 1;
        if (pic.index <= index) {
            it = pic.it;
            i = pic.index;
        }

        cout << i << " " << index << Nl;

        while (i++ < index) it++;
        this->pic = Itercache(it, i);
        // return *it;

        // cout << " ---" << it->priority << " " << it->value << Nl;

        // return it;
        return (*(it));
    };

    // std::list<Task>& getPendingTasks() {}

    Task getDoneTask(int index) {
        int listsz = getDoneListSize();
        if( ((index) > listsz) || index < 1 ) 
            // return nullptr;
            return Task(-1, "");

        auto it = doneTasks.begin();
        int i = 1;
        if (dic.index <= index) {
            it = dic.it;
            i = dic.index;
        }
        while (i++ < index) it++;
        dic = Itercache(it, i-1);
        // return *it;

        // return it;
        return (*(it));
    };

    bool addToDoneTasks(const Task& t) { 
        if (doneTasks.empty()) {
            doneTasks.push_front(t);
        } else {
            doneTasks.push_back(t);
        }
        return true;
    }

    bool addToPendingTasks(const Task& t) {
        /* Lower number first */
        std::list<Task>::reverse_iterator tail = pendingTasks.rbegin();
        std::list<Task>::iterator head = pendingTasks.begin();
        if (pendingTasks.empty() || tail->priority < t.priority) {
            pendingTasks.push_back(t);
        } else if (head->priority > t.priority) {
            pendingTasks.push_front(t);
        } else {

            auto prev = head;
            while( 1 ) {
                // if (head->priority < t.priority) break;
                // if (head != pendingTasks.end()) break;
                if (head == pendingTasks.end()) 
                    break;
                if (head->priority < t.priority) 
                    break;
                prev = head;
                ++head;
            }
            // pendingTasks.insert_after(prev, t);
            pendingTasks.insert(prev, t);
        }
        return true;
    }

    bool removePendingTask(const int index) { 
        
        if ( pendingTasks.empty() 
            || (index > getPendingListSize()) 
            || (index < 1)) {

            return false;
        } 

        /* 1-based indexing */
        std::list<Task>::iterator it = pendingTasks.begin();
        std::advance(it, index-1);
        pendingTasks.erase(it);

        // std::list<Task>::iterator cur = pendingTasks.begin();
        // std::list<Task>::iterator prev = cur;
        
        // if(index == 1) {
        //     pendingTasks.pop_front();
        // } else {
        //     index -= 1;
        //     while(--index) {
        //         prev = cur;
        //         cur++;
        //     }
            
        //     pendingTasks.erase_after(prev);
        // }
        return true;
    }

    bool removeDoneTask(const int index) { 
        
        if ( doneTasks.empty() 
            || (index > getDoneListSize()) 
            || (index < 1)) {
            return false;
        } 
        
        /* 1-based indexing */
        std::list<Task>::iterator it = doneTasks.begin();
        std::advance(it, index-1);
        doneTasks.erase(it);

        // std::forward_list<Task>::iterator cur = doneTasks.begin();
        // std::forward_list<Task>::iterator prev = cur;
        
        // if(index == 1) {
        //     doneTasks.pop_front();
        // } else {
        //     index -= 1;
        //     while(--index) {
        //         prev = cur;
        //         cur++;
        //     }

        //     doneTasks.erase_after(prev);
        // }
        return true;
    }

    bool loadDataD() { 

        std::ifstream file(this->donefile);
        // if (!file) {
        //     return false;
        // }
        if (!file) {
            // Create the file with write mode
            std::ofstream createFile(this->donefile);
            createFile.close();  // Close immediately after creation

            // Try opening again in read mode
            file.open(this->donefile);
            if (!file) {
                // File creation failed
                return false;
            }
        }

        std::string line;
        while (std::getline(file, line)) {
            Task t(0, line);
            addToDoneTasks(t);
        }

        return true;
    }

    bool unloadDataD() {         
        std::ofstream file(this->donefile);
        if (!file) {
            return false;
        }

        for (const Task& t : doneTasks) {
            file << t.value << Nl;
        }
        return true;

    }


    bool loadDataP() {
        std::ifstream file(this->taskfile);
        // if (!file) {
        //     return false;
        // }
        if (!file) {
            // Create the file with write mode
            std::ofstream createFile(this->taskfile);
            createFile.close();  // Close immediately after creation

            // Try opening again in read mode
            file.open(this->taskfile);
            if (!file) {
                // File creation failed
                return false;
            }
        }
        int pri;
        std::string line;
        while (std::getline(file, line)) {
            // Use std::istringstream for parsing
            std::istringstream iss(line);
            iss >> pri;  // Extract priority
            if (!iss) {  // Check for successful extraction
                // Handle parsing error (e.g., log, throw exception)
                return false;
            }
            std::getline(iss, line);  // Extract value
            Task t(pri, line);
            addToPendingTasks(t);
        }
        return true;
    }

    bool unloadDataP() {
        std::ofstream file(this->taskfile);
        if (!file) {
            return false;  // Handle file opening error
        }

        for (const Task& task : pendingTasks) {
            file << task.priority << " " << task.value << std::endl;
        }
        return true;
    }
};


ERRORS reportError(ERRORS err) {
    switch(err) {
        case SUCCESS:
            break;
        case NEGATIVE_PRIO:
            break;
        case MISSING_ADD_ARGS:
            std::cout << u8"Error: Missing tasks string. Nothing added!" << Nl;
            break;
        case MISSING_DEL_ARGS:
            std::cout << u8"Error: Missing NUMBER for deleting tasks." << Nl;
            break;
        case MISSING_DON_ARGS:
            std::cout << u8"Error: Missing NUMBER for marking tasks as done." << Nl;
            break;
        case NO_TASK_DONE:
            std::cout << u8"Error: no incomplete item with index #" << INDEX 
            << " exists." << Nl;
            break;
        case NO_TASK_DEL:
            std::cout << u8"Error: task with index #" << INDEX << "does not exist. \
            Nothing deleted." << Nl;
            break;
        default:
            std::cout << u8"Unknown error!" << Nl;
        }
    return err;
}


void help() {
    std::array<std::string, 6> examples {
        "add 2 hello world",
        "ls",
        "del INDEX",
        "done INDEX",
        "help",
        "report",
    };
    std::array<std::string, 6> descriptions {
        "Add a new item with priority 2 and text \"hello world\" to the list",
        "Show incomplete priority list items sorted by priority in ascending order",
        "Delete the incomplete item with the given index",
        "Mark the incomplete item with the given index as complete",
        "Show usage",
        "Statistics",
    };

    const int PADDING = 22;
    size_t i = 0;
    std::cout << "Usage :-\n";
    for (std::string example : examples) {

        std::cout << "$ " << "." + g_appname
            << std::setw(PADDING) 
            << std::left 
            << " " + example 
            << "# " << ( (i < descriptions.size()) ? descriptions[i] : "" ) 
            << "\n";

        i += 1;
    }

}


// void debug(PriorityTaskList &p) {
//     p.loadDataP();
//     std::cout << "Plist size " << p.getPendingListSize() << Nl;
//     p.loadDataD();
//     std::cout << "Dlist size " << p.getDoneListSize() << Nl;
// }


// inline void init(const int arg_count, char * args[], const char* tfile,
void init(size_t arg_count, char * args[]) {
    g_argc = arg_count;
    // g_argv = args;
    // std::cout << arg_count << Nl;
    // std::cout << args[0] << strlen(args[0]) <<  Nl;
    // std::string app = std::string(args[0], strlen(args[0]));
    // std::cout << app << " " << app.size() << Nl;
    size_t i;
    for (i = 0; i < arg_count; ++i) {
        // std::cout << args[i] << Nl;
        // g_argv.push_back(args[i]);
        g_argv.push_back(std::string(args[i], strlen(args[i])));

    }
    const size_t MIN_FILLING = 4;
    for (; i < MIN_FILLING; ++i) {
        g_argv.push_back("0");
    }
    
    
    g_appname = g_argv[0];
    
    size_t pos;
    pos = g_appname.find_last_of('/');
    if (pos != g_appname.npos) {
        g_appname = g_appname.substr(pos);
    }
    pos = g_appname.find_last_of('\\');
    if (pos != g_appname.npos) {
        g_appname = g_appname.substr(pos);
    }
    
    pos = g_appname.find_last_of('.');
    if (pos != g_appname.npos) {
        g_appname = g_appname.substr(0, pos);
    }
    
}

//reads and writes pending tasks
ERRORS addTask(PriorityTaskList *p, int priority, std::string input, bool (*missing_args)()) {

    //check error
    // if(g_argc < 4) {
    //     return ERRORS::MISSING_ADD_ARGS ;
    // }

    if (missing_args()) {
        // std::cout << "ERRRRRR" << Nl;
        return ERRORS::MISSING_ADD_ARGS;
    }

    
    p->loadDataP();
    p->addToPendingTasks(Task(priority, input) );
    p->unloadDataP();
    std::cout << "Added task: \"" << TASK << "\" with priority " << PRIORITY << Nl;

    // std::printf("Added task: \"%s\" with priority %s\n", TASK, PRIORITY);

    return ERRORS::SUCCESS;
}

//reads and writes pending tasks
ERRORS deleteTask(PriorityTaskList *p, int index, bool (*missing_args)()) {

    // if (g_argc < 3) {
    if (missing_args()) {
        return ERRORS::MISSING_DEL_ARGS;
    }

    p->loadDataP();
    if( !p->removePendingTask(index) ) 
        return ERRORS::NO_TASK_DEL;
    p->unloadDataP();
    std::cout << "Deleted task #" << index << Nl;
    // std::printf("Deleted task #%s\n", index);

    return ERRORS::SUCCESS;

}

//reads pendingTasks
void listTasks(PriorityTaskList& p) {
    
    p.loadDataP();

    int p_sz = p.getPendingListSize();
    if (p_sz == 0) {
        std::cout << "There are no pending tasks!";
        return;
    } 

    int i = 1;
    while(i <= p_sz) {

        Task t = p.getPendingTask(i);
        // std::cout << (i) <<  ". " << t.value << " [" << t.priority << "]\n";
        std::cout << (i) <<  ". " << t.value << " [" << t.priority << "]" << Nl;
        // std::cout << (i) <<  ". " << t->value << " [" << t->priority << "]" << Nl;
        i++;
    }
    std::cout << std::endl;

}

//moves task from one list to another; reads and writes both lists
ERRORS done(PriorityTaskList *p, int index, bool (*missing_args)()) {
    //check if index is less than 1 or greater than list size
    // if (g_argc < 3) 
    if (missing_args()) {
        return ERRORS::MISSING_DON_ARGS;
    }

    p->loadDataP();
    p->loadDataD();

    if ( (index > p->getPendingListSize()) || (index < 1) ) {
#if 0
        std::cout << p->getPendingListSize() << NL;
        std::cout << index << NL;

#endif
        return ERRORS::NO_TASK_DONE;
    }

#if 0
    std::cout << 23 << std::endl;
    std::cout << p->getPendingTask(index).value;
    p->addToDoneTasks( *(new Task(2, "manual labor")) );
#endif
    // p->addToDoneTasks(*(p->getPendingTask(index)));
    p->addToDoneTasks(p->getPendingTask(index));
    p->removePendingTask(index);

    p->unloadDataP();
    p->unloadDataD();

    // std::printf("Marked item as done.\n");
    std::cout << "Marked item as done." << Nl;
    
    return ERRORS::SUCCESS;    
}

//reads both lists
void report(PriorityTaskList* p) {
    p->loadDataP();
    p->loadDataD();

    int p_sz = p->getPendingListSize();
    int d_sz = p->getDoneListSize();

    // std::cout << "$." + g_appname << " report" << std::endl;
    std::cout << u8"Pending : " << p_sz << Nl;
    int i = 1;
    while(i <= p_sz) {
        Task t = p->getPendingTask(i);
        std::cout << i << ". " << t.value << " [" << t.priority << "]" << Nl;

        // std::cout << i << ". " << t.value << " [" << t.priority << "]\n";
        // std::cout << i << u8". " << t->value << u8" [" << t->priority << u8"]" << Nl;
        i++;
    }
    std::cout << std::endl;

    std::cout << "Completed : " << d_sz << Nl;
    i = 1;
    while(i <= d_sz) {
        Task t = p->getDoneTask(i);
        std::cout << i << ". " << t.value << Nl;
        // std::cout << i << ". " << t->value << Nl;
        i++;
    }
    std::cout << std::endl;

}

void debug(PriorityTaskList &p) {
    p.loadDataP();
    std::cout << "Plist size " << p.getPendingListSize() << Nl;
    p.loadDataD();
    std::cout << "Dlist size " << p.getDoneListSize() << Nl;
}

int main(int argc, char * argv[]) {

    // init(argc, argv, "task.txt", "completed.txt");
    init(argc, argv);

    /* 
    https://github.com/nseadlc-2020/package-todo-cli-task/issues/12
    https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
    */
    // SetConsoleOutputCP( CP_UTF8 );
    // setvbuf(stdout, nullptr, _IOFBF, 1000);

#ifdef DEBUG
    std::cout << "(cout-width) " << std::cout.width() << Nl;
    std::cout << g_argc << Nl;
    std::cout << g_argv[2] << Nl;
    ptl.getPendingTask(2);
#else

    if (argc == 1) {
        help();
        return EXIT_SUCCESS;
    }


    enum SUBCMD {ADD = 1, DEL, DON, REPO, LIST, HELP, DEBUG};

    std::unordered_map<std::string, SUBCMD> cmdmap = {
        {"add", SUBCMD::ADD},
        {"del", SUBCMD::DEL},
        {"report", SUBCMD::REPO},
        {"ls", SUBCMD::LIST},
        {"done", SUBCMD::DON},
        {"help", SUBCMD::HELP},
        {"debug", SUBCMD::DEBUG}

    };

    PriorityTaskList ptl("tasks.txt", "completed.txt");

    switch(cmdmap[COMMAND]){
        case ADD:
            
            return reportError(addTask(&ptl, std::stoi(PRIORITY), TASK, []() {
                return g_argc < 4;
            }));
        case DEL:
            return reportError(deleteTask(&ptl, std::stoi(INDEX), []() {
                return g_argc < 3;
            }));
        case DON:
            return reportError(done(&ptl, std::stoi(INDEX), []() {
                return g_argc < 3;
            }));
        case REPO:
            report(&ptl);
            break;
        case LIST:
            listTasks(ptl);
            break;
        case HELP:
            help();
            break;
        case DEBUG:
            debug(ptl);
            break;
        default:
            // return reportError(0);
            std::cout << "No command found... \n";
            break;
    }
#endif
    return EXIT_SUCCESS;
}