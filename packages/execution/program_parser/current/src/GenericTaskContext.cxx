/***************************************************************************
  tag: Peter Soetens  Sat May 7 12:56:52 CEST 2005  GenericTaskContext.cxx 

                        GenericTaskContext.cxx -  description
                           -------------------
    begin                : Sat May 07 2005
    copyright            : (C) 2005 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/
 
 

#include <rtt/GenericTaskContext.hpp>

#include <rtt/ProgramLoader.hpp>
#include <rtt/PropertyLoader.hpp>

// To create commands, methods, etc :
#include <rtt/Method.hpp>

namespace RTT
{
    GenericTaskContext::GenericTaskContext(const std::string& name)
        : TaskContext( name )
    {
        // See Task Infrastructure Chapter.

        // Register our methods :
        methods()->addMethod( method("assert",
                    &GenericTaskContext::assertion, this), "Assert will get your program in the error state if the argument is false.",
                            "MustBeTrue", "The result of a boolean expression which must be true.");
        methods()->addMethod( method("start",
                    &GenericTaskContext::start, this), "Start this GenericTaskContext." );
        methods()->addMethod( method("stop",
                    &GenericTaskContext::stop, this), "Stop this GenericTaskContext." );
        methods()->addMethod( method("readProperties",
                    &GenericTaskContext::readProperties, this), "Read Properties from a file.",
                            "Filename", "A CPF formatted XML file.");
        methods()->addMethod( method("writeProperties",
                    &GenericTaskContext::writeProperties, this), "Write or update Properties to a file.",
                            "Filename", "A CPF formatted XML file.");
        methods()->addMethod( method("loadProgram",
                    &GenericTaskContext::loadProgram, this), "Load an Orocos Program Script from a file.",
                            "Filename", "An ops file.");
        methods()->addMethod( method("loadStateMachine",
                    &GenericTaskContext::loadStateMachine, this), "Load an Orocos State Description from a file.",
                            "Filename", "An osd file.");
        methods()->addMethod( method("unloadProgram",
                    &GenericTaskContext::unloadProgram, this), "Unload an Orocos Program Script from a file.",
                            "ProgramName", "The Program's name.");
        methods()->addMethod( method("unloadStateMachine",
                    &GenericTaskContext::unloadStateMachine, this), "Unload an Orocos State Description from a file.",
                            "StateMachineName", "A name of a Root StateMachine instantiation.");
        methods()->addMethod( method("isRunning",
                    &GenericTaskContext::isRunning, this), "Is this GenericTaskContext started ?" );
        methods()->addMethod( method("update",
                    &GenericTaskContext::doUpdate, this), "Invoke the update method. Only succeeds if the task isRunning()." );
    }

#if 0
    GenericTaskContext::createScriptingObject()
    {
        TaskObject* to =  new TaskObject("scripting");
        to->methods()->addMethod( method("loadProgram",
                    &GenericTaskContext::loadProgram, this), "Load an Orocos Program Script from a file.",
                            "Filename", "An ops file.");
        to->methods()->addMethod( method("loadStateMachine",
                    &GenericTaskContext::loadStateMachine, this), "Load an Orocos State Description from a file.",
                            "Filename", "An osd file.");
        to->methods()->addMethod( method("unloadProgram",
                    &GenericTaskContext::unloadProgram, this), "Unload an Orocos Program Script from a file.",
                            "ProgramName", "The Program's name.");
        to->methods()->addMethod( method("unloadStateMachine",
                    &GenericTaskContext::unloadStateMachine, this), "Unload an Orocos State Description from a file.",
                            "StateMachineName", "A name of a Root StateMachine instantiation.");
    }
#endif

    GenericTaskContext::~GenericTaskContext()
    {}

    bool GenericTaskContext::assertion(bool mbt)
    {
        return mbt;
    }

    bool GenericTaskContext::doUpdate()
    {
        if ( this->engine()->getActivity() == 0 )
            return false;
        return this->engine()->getActivity()->update();
    }

    /**
     * Start is a method which starts the Processor's task.
     * It can not be a command because if the Processor is not running,
     * it does not accept commands. Also, RunnableInterface::initialize()
     * is then called in the context of the caller.
     */
    bool GenericTaskContext::start() {
        if ( this->engine()->getActivity() == 0 )
            return false;
        return this->engine()->getActivity()->start();
    }
        
    /**
     * Stop is a method which stops the Processor's task.
     * RunnableInterface::finalize()
     * is called in the context of the caller.
     */
    bool GenericTaskContext::stop() {
        if ( this->engine()->getActivity() == 0 )
            return false;
        return this->engine()->getActivity()->stop();
    }
  
    /**
     * DataSource to inspect if this Task is running.
     */
    bool GenericTaskContext::isRunning() const {
        if ( this->engine()->getActivity() == 0 )
            return false;
        return this->engine()->getActivity()->isRunning();
    }

    /**
     * Read this Task's properties from a file.
     */
    bool GenericTaskContext::readProperties(const std::string& filename) {
        PropertyLoader p;
        return p.configure(filename, this);
    }

    /**
     * Write this Task's properties to a file.
     */
    bool GenericTaskContext::writeProperties(const std::string& filename) {
        PropertyLoader p;
        return p.save(filename, this);
    }

    /**
     * Load an Orocos Program Script from disk.
     */
    bool GenericTaskContext::loadProgram(const std::string& filename) {
        ProgramLoader p;
        return p.loadProgram(filename, this);
    }

    /**
     * Load an Orocos State Description from disk.
     */
    bool GenericTaskContext::loadStateMachine(const std::string& filename) {
        ProgramLoader p;
        return p.loadStateMachine(filename, this);
    }

    /**
     * Unload an Orocos Program Script from disk.
     */
    bool GenericTaskContext::unloadProgram(const std::string& progname) {
        ProgramLoader p;
        return p.unloadProgram(progname, this);
    }

    /**
     * Unload an Orocos State Description from disk.
     */
    bool GenericTaskContext::unloadStateMachine(const std::string& instancename) {
        ProgramLoader p;
        return p.unloadStateMachine(instancename, this);
    }

}

