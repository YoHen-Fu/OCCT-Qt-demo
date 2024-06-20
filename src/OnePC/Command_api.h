#ifndef COMMAND_API_H
#define COMMAND_API_H

#include <QObject>
#include <QAction>

#include "IAppContext.h"

class Command : public QObject {
    Q_OBJECT
public:
    Command(IAppContext* appContext);
    virtual ~Command() = default;
    virtual void execute() = 0;
    void setAction(QAction* action);
    QAction* action() const { return m_action; }
protected:
    IAppContext* getappContext() const { return m_appContext; }
private:
    IAppContext* m_appContext = nullptr;
    QAction* m_action = nullptr;
};

class Command_ShowPC : public Command{
public:
    Command_ShowPC(IAppContext* appContext);
    void execute();
    static constexpr std::string_view Name = "show PC";
};

class Command_HidePC : public Command{
public:
    Command_HidePC(IAppContext* appContext);
    void execute();
    static constexpr std::string_view Name = "hide PC";
};

class Command_ViewFit : public Command{
public:
    Command_ViewFit(IAppContext* appContext);
    void execute();
    static constexpr std::string_view Name = "view fit";
};
class Command_ViewFront : public Command{
public:
    Command_ViewFront(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "view front";
};
class Command_ViewLeft : public Command{
public:
    Command_ViewLeft(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "view left";
};
class Command_ViewTop : public Command{
public:
    Command_ViewTop(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "view top";
};
class Command_ViewIsometric : public Command{
public:
    Command_ViewIsometric(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "view isometric";
};
class Command_OpenPCAct : public Command{
public:
    Command_OpenPCAct(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "open PC";
};
class Command_ClosePCAct : public Command{
public:
    Command_ClosePCAct(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "close PC";
};
class Command_ShowFrame : public Command{
public:
    Command_ShowFrame(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "show frame";
};
class Command_HideFrame : public Command{
public:
    Command_HideFrame(IAppContext* guiWidget);
    void execute();
    static constexpr std::string_view Name = "hide frame";
};


class CommandContainer{
public:
    CommandContainer() = default;

    CommandContainer(IAppContext* appContext);

    void setAppContext(IAppContext* appContext);

    template<typename Function> void foreachCommand(Function fn);

    // Returns the Command object mapped to 'name'
    // That object was previously created and associated with a call to addCommand()/addNamedCommand()
    // Might return null in case no command is mapped to 'name'
    Command* findCommand(std::string_view name) const;

    // Helper function to retrieve the action provided by the Command object mapped to 'name'
    // Might return null in case no command is mapped to 'name'
    QAction* findCommandAction(std::string_view name) const;

    // Construct and add new Command object with arguments 'args'
    // The command is associated to identigfier 'name' and can be retrieved later on with findCommand()
    template<typename CmdType, typename... Args> CmdType* addCommand(std::string_view name, Args... p);

    // Same behavior as addCommand() function
    // The command name is implicit and found by assuming the presence of CmdType::Name class member
    template<typename CmdType, typename... Args> CmdType* addNamedCommand(Args... p);

    void clear();

private:
    void addCommand_impl(std::string_view name, Command* cmd);
    IAppContext* m_appContext = nullptr;
    std::unordered_map<std::string_view, Command*> m_mapCommand;
};

template<typename Function>
void CommandContainer::foreachCommand(Function fn)
{
    for (auto [name, cmd] : m_mapCommand) {
        fn(name, cmd);
    }
}

template<typename CmdType, typename... Args>
CmdType* CommandContainer::addCommand(std::string_view name, Args... p)
{
    auto cmd = new CmdType(p...);
    this->addCommand_impl(name, cmd);
    return cmd;
}

template<typename CmdType, typename... Args>
CmdType* CommandContainer::addNamedCommand(Args... p)
{;
    auto cmd = new CmdType(m_appContext, p...);
    this->addCommand_impl(CmdType::Name, cmd);
    return cmd;
}


#endif // COMMAND_API_H
