/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:26 CET 2004  BaseComponents.hpp 

                        BaseComponents.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
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
 
#ifndef BASECOMPONENTS_HPP
#define BASECOMPONENTS_HPP

#include <corelib/DataObjectInterfaces.hpp>
#include "ComponentInterfaces.hpp"
#include "PortInterfaces.hpp"

namespace ORO_ControlKernel
{
    using ORO_CoreLib::DataObjectInterface;

    /**
     * @defgroup kcomps All Available Control Kernel Components.
     */

    /**
     * @defgroup kcomp_support All Available ControlKernel Support Components
     */

    /**
     * @brief A SupportComponent does not 'run' actually in the control loop,
     * but it gets the same functionality as any other control component.
     * @ingroup kcomps kcomp_support
     */
    template < class _Facet = DefaultBase >
    class SupportComponent
        :  public _Facet
    {
    public:
        typedef _Facet Facet;

        /**
         * A Support Component.
         */
        SupportComponent(const std::string& name ) 
            : Facet( name )
        {}

        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->supports.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->supports.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->supports.unregisterObject( this );
        }

        template< class KernelT >
        void createPorts(KernelT*  ) {
        }

        void erasePorts() {
        }

        template< class KernelT >
        bool select(KernelT* ) {
            return true;
        }

        template< class KernelT >
        void createDataObject( KernelT* ) {
        }

    };

    /**
     * @defgroup kcomp_controller All Available ControlKernel Controller Components
     */

    /**
     * @brief A controller defined by the topological connections to Data Objects and
     * its ports to it.
     * @ingroup kcomps kcomp_controller
     */
    template <class _InputType, class _ModelType, class _SetPointType, class _OutputType, class _Facet = DefaultBase >
    class Controller
        : public _Facet
    {
    public:
        typedef typename _SetPointType::DataType SetPointType;
        typedef typename _InputType::DataType InputType;
        typedef typename _ModelType::DataType ModelType;
        typedef typename _OutputType::DataType OutputType;
        typedef _Facet Facet;

        typedef typename _SetPointType::ReadPort SetPointPort;
        typedef typename _InputType::ReadPort InputPort;
        typedef typename _ModelType::ReadPort ModelPort;
        typedef typename _OutputType::WritePort OutputPort;

        Controller(const std::string& name ) 
            : Facet( name ),
              SetPoint( new SetPointPort() ),
              Input( new InputPort() ),
              Model( new ModelPort() ),
              Output( new OutputPort() )
        {}
        
        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->controllers.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->controllers.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->controllers.unregisterObject( this );
        }

        template< class KernelT >
        void createDataObject( KernelT* k) {
            Output->createDataObject( k->getKernelName()+"::Outputs", k->getOutputPrefix(),typename KernelT::OutputPortType()  );
        }

        OutputPort* writePort() {
            return Output;
        }

        template< class KernelT >
        void createPorts(KernelT* k ) {
            Input->createPort( k->getKernelName()+"::Inputs", k->getInputPrefix() );
            Model->createPort( k->getKernelName()+"::Models", k->getModelPrefix() );
            SetPoint->createPort( k->getKernelName()+"::SetPoints", k->getSetPointPrefix() );
            Output->createPort( k->getKernelName()+"::Outputs", k->getOutputPrefix() );
        }

        void erasePorts() {
            Input->erasePort();
            Model->erasePort();
            SetPoint->erasePort();
            Output->erasePort();
        }

        template< class KernelT >
        bool select(KernelT* k) {
            return k->selectController( this );
        }

        SetPointPort* SetPoint;
        InputPort*  Input;
        ModelPort*  Model;
        OutputPort* Output;
    };

    /**
     * @defgroup kcomp_generator All Available ControlKernel Generator Components
     */

    /**
     * @brief A generator defined by the topological connections to Data Objects and
     * its ports to it.
     * @ingroup kcomps kcomp_generator
     */
    template <class _InputType, class _ModelType, class _CommandType, class _SetPointType, class _Facet = DefaultBase >
    class Generator
        : public _Facet
    {
    public:
        typedef typename _CommandType::DataType CommandType;
        typedef typename _SetPointType::DataType SetPointType;
        typedef typename _InputType::DataType InputType;
        typedef typename _ModelType::DataType ModelType;
        typedef _Facet Facet;

        typedef typename _CommandType::ReadPort CommandPort;
        typedef typename _InputType::ReadPort InputPort;
        typedef typename _ModelType::ReadPort ModelPort;
        typedef typename _SetPointType::WritePort SetPointPort;

        Generator(const std::string& name ) 
            : Facet( name ),
              Command( new CommandPort() ),
              Input( new InputPort() ),
              Model( new ModelPort() ),
              SetPoint( new SetPointPort() )
        {}

        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->generators.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->generators.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->generators.unregisterObject( this );
        }

        template< class KernelT >
        void createDataObject( KernelT* k) {
            SetPoint->createDataObject( k->getKernelName()+"::SetPoints", k->getSetPointPrefix(), typename KernelT::SetPointPortType() );
        }


        SetPointPort* writePort() {
            return SetPoint;
        }

        template< class KernelT >
        void createPorts(KernelT* k ) {
            Input->createPort( k->getKernelName()+"::Inputs", k->getInputPrefix() );
            Model->createPort( k->getKernelName()+"::Models", k->getModelPrefix() );
            SetPoint->createPort( k->getKernelName()+"::SetPoints", k->getSetPointPrefix() );
            Command->createPort( k->getKernelName()+"::Commands", k->getCommandPrefix() );
        }

        void erasePorts() {
            Command->erasePort();
            Input->erasePort();
            Model->erasePort();
            SetPoint->erasePort();
        }

        template< class KernelT >
        bool select(KernelT* k) {
            return k->selectGenerator( this );
        }

        CommandPort* Command;
        InputPort*  Input;
        ModelPort*  Model;
        SetPointPort* SetPoint;
    };

    /**
     * @defgroup kcomp_estimator All Available ControlKernel Estimator Components
     */

    /**
     * @brief An estimator defined by the topological connections to Data Objects and
     * its ports to it.
     * @ingroup kcomps kcomp_estimator
     */
    template <class _InputType, class _OutputType, class _ModelType, class _Facet = DefaultBase >
    class Estimator
        : public _Facet
    {
    public:
        typedef typename _InputType::DataType InputType;
        typedef typename _OutputType::DataType OutputType;
        typedef typename _ModelType::DataType ModelType;
        typedef _Facet Facet;
            
        typedef typename _InputType::ReadPort InputPort;
        typedef typename _OutputType::ReadPort OutputPort;
        typedef typename _ModelType::WritePort ModelPort;

        Estimator(const std::string& name ) 
            : Facet( name ),
              Input( new InputPort() ),
              Output( new OutputPort() ),
              Model( new ModelPort() )
        {}

        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->estimators.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->estimators.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->estimators.unregisterObject( this );
        }

        template< class KernelT >
        void createDataObject( KernelT* k) {
            Model->createDataObject( k->getKernelName()+"::Models", k->getModelPrefix(), typename KernelT::ModelPortType() );
        }

        ModelPort* writePort() {
            return Model;
        }

        template< class KernelT >
        void createPorts( KernelT* k ) {
            Input->createPort( k->getKernelName()+"::Inputs", k->getInputPrefix() );
            Output->createPort( k->getKernelName()+"::Outputs", k->getOutputPrefix() );
            Model->createPort( k->getKernelName()+"::Models", k->getModelPrefix() );
        }

        void erasePorts() {
            Input->erasePort();
            Output->erasePort();
            Model->erasePort();
        }

        template< class KernelT >
        bool select(KernelT* k) {
            return k->selectEstimator( this );
        }

        InputPort*  Input;
        OutputPort* Output;
        ModelPort*  Model;
    };


    /**
     * @defgroup kcomp_effector All Available ControlKernel Effector Components
     */

    /**
     * @brief An effector defined by the topological connections to Data Objects and
     * its ports to it.
     * @ingroup kcomps kcomp_effector
     */
    template <class _InputType, class _ModelType, class _OutputType, class _Facet = DefaultBase >
    class Effector
        : public _Facet
    {
    public:
        typedef typename _InputType::DataType InputType;
        typedef typename _ModelType::DataType ModelType;
        typedef typename _OutputType::DataType OutputType;
        typedef _Facet Facet;

        typedef typename _InputType::ReadPort InputPort;
        typedef typename _ModelType::ReadPort ModelPort;
        typedef typename _OutputType::ReadPort OutputPort;

        Effector(const std::string& name ) 
            : Facet( name ),
              Input( new InputPort() ),
              Model( new ModelPort() ),
              Output( new OutputPort() )
        {}

        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->effectors.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->effectors.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->effectors.unregisterObject( this );
        }

        template< class KernelT >
        void createDataObject( KernelT* k) {
            //NOP
        }

        OutputPort* writePort() {
            // ok, this is a bit weird, the Effector has no
            // write ports but we must implement this method (generic programming stuff)
            // so I return zero and check for zero when calling writePort().
            return 0;
        }

        template< class KernelT >
        void createPorts(KernelT* k ) {
            Input->createPort( k->getKernelName()+"::Inputs", k->getInputPrefix() );
            Model->createPort( k->getKernelName()+"::Models", k->getModelPrefix() );
            Output->createPort( k->getKernelName()+"::Outputs", k->getOutputPrefix() );
        }

        void erasePorts() {
            Input->erasePort();
            Model->erasePort();
            Output->erasePort();
        }

        template< class KernelT >
        bool select(KernelT* k) {
            return k->selectEffector( this );
        }

        InputPort*  Input;
        ModelPort*  Model;
        OutputPort* Output;
    };

    /**
     * @defgroup kcomp_sensor All Available ControlKernel Sensor Components
     */

    /**
     * @brief A sensor defined by the topological connections to Data Objects and
     * its ports to it.
     * @ingroup kcomps kcomp_sensor
     */
    template <class _InputType, class _Facet = DefaultBase >
    class Sensor
        : public _Facet
    {
    public:
        typedef typename _InputType::DataType  InputType;
        typedef _Facet Facet;
            
        typedef typename _InputType::WritePort InputPort;

        Sensor(const std::string& name ) 
            : Facet( name ),
              Input ( new InputPort() )
        {}

        template< class KernelT >
        bool enableFacets(KernelT* k) {
            return k->sensors.registerObject( this, this->getName() ) && ( _Facet::enableFacet(k) || (k->sensors.unregisterObject( this ), false) );
        }

        template< class KernelT >
        void disableFacets(KernelT* k) {
             _Facet::disableFacet();
             k->sensors.unregisterObject( this );
        }

        template< class KernelT >
        void createDataObject( KernelT* k) {
            Input->createDataObject( k->getKernelName()+"::Inputs", k->getInputPrefix(),typename KernelT::InputPortType() );
        }

        template< class KernelT >
        void createPorts( KernelT* k ) {
            Input->createPort( k->getKernelName()+"::Inputs", k->getInputPrefix() );
        }

        void erasePorts() {
            Input->erasePort();
        }

        template< class KernelT >
        bool select(KernelT* k) {
            return k->selectSensor( this );
        }

        InputPort* writePort() {
            return Input;
        }

        // The InputPort.
        InputPort* Input;
        
    };
                
}

#endif
