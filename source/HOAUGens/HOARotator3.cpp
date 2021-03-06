/* ------------------------------------------------------------
author: "Pierre Lecomte"
copyright: "(c) Pierre Lecomte 2016"
license: "GPL"
name: "HOARotator3"
version: "1.0"
Code generated with Faust 2.5.21 (https://faust.grame.fr)
Compilation options: cpp, -double -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

//-------------------------------------------------------------------
// FAUST architecture file for SuperCollider.
// Copyright (C) 2005-2012 Stefan Kersten.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA
//-------------------------------------------------------------------

// The prefix is set to "Faust" in the faust2supercollider script, otherwise set empty
#if !defined(SC_FAUST_PREFIX)
#define SC_FAUST_PREFIX ""
#endif

#include <map>
#include <string>
#include <string.h>
#include <SC_PlugIn.h>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

class UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'addBtton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void init(int samplingRate) = 0;

        /**
         * Init instance state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceInit(int samplingRate) = 0;

        /**
         * Init instance constant state
         *
         * @param samplingRate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int samplingRate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual dsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = 0):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int samplingRate) { fDSP->init(samplingRate); }
        virtual void instanceInit(int samplingRate) { fDSP->instanceInit(samplingRate); }
        virtual void instanceConstants(int samplingRate) { fDSP->instanceConstants(samplingRate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
    
        virtual dsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

class UI
{

    public:

        UI() {}

        virtual ~UI() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) = 0;
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};

#endif
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/
 
#ifndef __misc__
#define __misc__

#include <algorithm>
#include <map>
#include <string.h>
#include <stdlib.h>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual void declare(const char* key, const char* value) = 0;
    virtual ~Meta() {};
};

#endif

using std::max;
using std::min;

struct XXXX_Meta : std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

struct MY_Meta : Meta, std::map<const char*, const char*>
{
    void declare(const char* key, const char* value) { (*this)[key]=value; }
};

inline int lsr(int x, int n)	{ return int(((unsigned int)x) >> n); }

inline int int2pow2(int x)		{ int r = 0; while ((1<<r) < x) r++; return r; }

inline long lopt(char* argv[], const char* name, long def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

inline bool isopt(char* argv[], const char* name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

inline const char* lopts(char* argv[], const char* name, const char* def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return argv[i+1];
	return def;
}

#endif


using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 4
    #define FAUST_EXPORT __attribute__((visibility("default")))
#else
    #define FAUST_EXPORT  SC_API_EXPORT
#endif

#ifdef WIN32
    #define STRDUP _strdup
#else
    #define STRDUP strdup
#endif

//----------------------------------------------------------------------------
// Vector intrinsics
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Metadata
//----------------------------------------------------------------------------

class MetaData : public Meta
               , public std::map<std::string, std::string>
{
public:
    void declare(const char* key, const char* value)
    {
        (*this)[key] = value;
    }
};

//----------------------------------------------------------------------------
// Control counter
//----------------------------------------------------------------------------

class ControlCounter : public UI
{
public:
    ControlCounter()
        : mNumControlInputs(0),
          mNumControlOutputs(0)
    {}

    size_t getNumControls() const { return getNumControlInputs(); }
    size_t getNumControlInputs() const { return mNumControlInputs; }
    size_t getNumControlOutputs() const { return mNumControlOutputs; }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addControlInput(); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addControlInput(); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    { addControlOutput(); }
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

protected:
    void addControlInput() { mNumControlInputs++; }
    void addControlOutput() { mNumControlOutputs++; }

private:
    size_t mNumControlInputs;
    size_t mNumControlOutputs;
};

//----------------------------------------------------------------------------
// UI control
//----------------------------------------------------------------------------

struct Control
{
    typedef void (*UpdateFunction)(Control* self, FAUSTFLOAT value);

    UpdateFunction updateFunction;
    FAUSTFLOAT* zone;
    FAUSTFLOAT min, max;

    inline void update(FAUSTFLOAT value)
    {
        (*updateFunction)(this, value);
    }

    static void simpleUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = value;
    }
    static void boundedUpdate(Control* self, FAUSTFLOAT value)
    {
        *self->zone = sc_clip(value, self->min, self->max);
    }
};

//----------------------------------------------------------------------------
// Control allocator
//----------------------------------------------------------------------------

class ControlAllocator : public UI
{
public:
    ControlAllocator(Control* controls)
        : mControls(controls)
    { }

    // Layout widgets
    virtual void openTabBox(const char* label) { }
    virtual void openHorizontalBox(const char* label) { }
    virtual void openVerticalBox(const char* label) { }
    virtual void closeBox() { }

    // Active widgets
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    { addSimpleControl(zone); }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    { addBoundedControl(zone, min, max, step); }

    // Passive widgets
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

private:
    void addControl(Control::UpdateFunction updateFunction, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT /* step */)
    {
        Control* ctrl        = mControls++;
        ctrl->updateFunction = updateFunction;
        ctrl->zone           = zone;
        ctrl->min            = min;
        ctrl->max            = max;
    }
    void addSimpleControl(FAUSTFLOAT* zone)
    {
        addControl(Control::simpleUpdate, zone, 0.f, 0.f, 0.f);
    }
    void addBoundedControl(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        addControl(Control::boundedUpdate, zone, min, max, step);
    }

private:
    Control* mControls;
};

//----------------------------------------------------------------------------
// FAUST generated code
//----------------------------------------------------------------------------

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <cmath>

double mydsp_faustpower2_f(double value) {
	return (value * value);
	
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif
#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class mydsp : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	int fSamplingFreq;
	
 public:
	
	void metadata(Meta* m) { 
		m->declare("author", "Pierre Lecomte");
		m->declare("copyright", "(c) Pierre Lecomte 2016");
		m->declare("license", "GPL");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.1");
		m->declare("name", "HOARotator3");
		m->declare("version", "1.0");
		m->declare("ymn.lib/author", "Pierre Lecomte");
		m->declare("ymn.lib/copyright", "(c) Pierre Lecomte 2017");
		m->declare("ymn.lib/license", "GPL");
		m->declare("ymn.lib/name", "Spherical Harmonics library");
		m->declare("ymn.lib/version", "2.0");
	}

	virtual int getNumInputs() {
		return 16;
		
	}
	virtual int getNumOutputs() {
		return 16;
		
	}
	virtual int getInputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
				rate = 1;
				break;
			}
			case 8: {
				rate = 1;
				break;
			}
			case 9: {
				rate = 1;
				break;
			}
			case 10: {
				rate = 1;
				break;
			}
			case 11: {
				rate = 1;
				break;
			}
			case 12: {
				rate = 1;
				break;
			}
			case 13: {
				rate = 1;
				break;
			}
			case 14: {
				rate = 1;
				break;
			}
			case 15: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	virtual int getOutputRate(int channel) {
		int rate;
		switch (channel) {
			case 0: {
				rate = 1;
				break;
			}
			case 1: {
				rate = 1;
				break;
			}
			case 2: {
				rate = 1;
				break;
			}
			case 3: {
				rate = 1;
				break;
			}
			case 4: {
				rate = 1;
				break;
			}
			case 5: {
				rate = 1;
				break;
			}
			case 6: {
				rate = 1;
				break;
			}
			case 7: {
				rate = 1;
				break;
			}
			case 8: {
				rate = 1;
				break;
			}
			case 9: {
				rate = 1;
				break;
			}
			case 10: {
				rate = 1;
				break;
			}
			case 11: {
				rate = 1;
				break;
			}
			case 12: {
				rate = 1;
				break;
			}
			case 13: {
				rate = 1;
				break;
			}
			case 14: {
				rate = 1;
				break;
			}
			case 15: {
				rate = 1;
				break;
			}
			default: {
				rate = -1;
				break;
			}
			
		}
		return rate;
		
	}
	
	static void classInit(int samplingFreq) {
		
	}
	
	virtual void instanceConstants(int samplingFreq) {
		fSamplingFreq = samplingFreq;
		
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = FAUSTFLOAT(0.0);
		fHslider1 = FAUSTFLOAT(0.0);
		fHslider2 = FAUSTFLOAT(0.0);
		
	}
	
	virtual void instanceClear() {
		
	}
	
	virtual void init(int samplingFreq) {
		classInit(samplingFreq);
		instanceInit(samplingFreq);
	}
	virtual void instanceInit(int samplingFreq) {
		instanceConstants(samplingFreq);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp();
	}
	virtual int getSampleRate() {
		return fSamplingFreq;
		
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("HOARotator3");
		ui_interface->declare(&fHslider2, "osc", "/picth 0 360");
		ui_interface->addHorizontalSlider("Pitch", &fHslider2, 0.0, -3.1415926535897931, 3.1415926535897931, 0.01);
		ui_interface->declare(&fHslider1, "osc", "/roll 0 360");
		ui_interface->addHorizontalSlider("Roll", &fHslider1, 0.0, -3.1415926535897931, 3.1415926535897931, 0.01);
		ui_interface->declare(&fHslider0, "osc", "/yaw 0 360");
		ui_interface->addHorizontalSlider("Yaw", &fHslider0, 0.0, -3.1415926535897931, 3.1415926535897931, 0.01);
		ui_interface->closeBox();
		
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* input6 = inputs[6];
		FAUSTFLOAT* input7 = inputs[7];
		FAUSTFLOAT* input8 = inputs[8];
		FAUSTFLOAT* input9 = inputs[9];
		FAUSTFLOAT* input10 = inputs[10];
		FAUSTFLOAT* input11 = inputs[11];
		FAUSTFLOAT* input12 = inputs[12];
		FAUSTFLOAT* input13 = inputs[13];
		FAUSTFLOAT* input14 = inputs[14];
		FAUSTFLOAT* input15 = inputs[15];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		FAUSTFLOAT* output2 = outputs[2];
		FAUSTFLOAT* output3 = outputs[3];
		FAUSTFLOAT* output4 = outputs[4];
		FAUSTFLOAT* output5 = outputs[5];
		FAUSTFLOAT* output6 = outputs[6];
		FAUSTFLOAT* output7 = outputs[7];
		FAUSTFLOAT* output8 = outputs[8];
		FAUSTFLOAT* output9 = outputs[9];
		FAUSTFLOAT* output10 = outputs[10];
		FAUSTFLOAT* output11 = outputs[11];
		FAUSTFLOAT* output12 = outputs[12];
		FAUSTFLOAT* output13 = outputs[13];
		FAUSTFLOAT* output14 = outputs[14];
		FAUSTFLOAT* output15 = outputs[15];
		double fSlow0 = double(fHslider0);
		double fSlow1 = cos(fSlow0);
		double fSlow2 = double(fHslider1);
		double fSlow3 = cos(fSlow2);
		double fSlow4 = (fSlow1 * fSlow3);
		double fSlow5 = sin(fSlow0);
		double fSlow6 = sin(fSlow2);
		double fSlow7 = (fSlow5 * fSlow6);
		double fSlow8 = double(fHslider2);
		double fSlow9 = sin(fSlow8);
		double fSlow10 = (fSlow4 - (fSlow7 * fSlow9));
		double fSlow11 = cos(fSlow8);
		double fSlow12 = (0.0 - fSlow11);
		double fSlow13 = (fSlow6 * fSlow12);
		double fSlow14 = (fSlow6 * fSlow9);
		double fSlow15 = (fSlow5 * fSlow3);
		double fSlow16 = ((fSlow14 * fSlow1) + fSlow15);
		double fSlow17 = (fSlow6 * fSlow1);
		double fSlow18 = (fSlow5 * fSlow9);
		double fSlow19 = (fSlow17 + (fSlow18 * fSlow3));
		double fSlow20 = (fSlow3 * fSlow11);
		double fSlow21 = (fSlow9 * fSlow1);
		double fSlow22 = ((fSlow21 * (0.0 - fSlow3)) + fSlow7);
		double fSlow23 = (fSlow5 * fSlow12);
		double fSlow24 = (fSlow1 * fSlow11);
		double fSlow25 = ((fSlow24 * fSlow10) + (fSlow23 * fSlow16));
		double fSlow26 = mydsp_faustpower2_f(fSlow12);
		double fSlow27 = ((fSlow9 * fSlow10) + (fSlow7 * fSlow26));
		double fSlow28 = (1.7320508075688772 * (fSlow14 * fSlow12));
		double fSlow29 = ((fSlow9 * fSlow16) + ((fSlow17 * fSlow11) * fSlow12));
		double fSlow30 = ((fSlow24 * fSlow16) - (fSlow23 * fSlow10));
		double fSlow31 = ((fSlow19 * fSlow16) + (fSlow22 * fSlow10));
		double fSlow32 = ((fSlow20 * fSlow10) + (1.0000000000000002 * ((fSlow6 * fSlow19) * fSlow12)));
		double fSlow33 = (1.7320508075688772 * (((fSlow6 * fSlow3) * fSlow11) * fSlow12));
		double fSlow34 = ((fSlow20 * fSlow16) + (1.0000000000000002 * ((fSlow6 * fSlow22) * fSlow12)));
		double fSlow35 = ((fSlow22 * fSlow16) - (fSlow19 * fSlow10));
		double fSlow36 = (((fSlow5 * fSlow1) * fSlow11) * fSlow12);
		double fSlow37 = (fSlow16 * fSlow10);
		double fSlow38 = ((1.1547005383792515 * (fSlow22 * fSlow19)) - (0.57735026918962573 * (fSlow36 + fSlow37)));
		double fSlow39 = (fSlow6 * fSlow10);
		double fSlow40 = ((1.1547005383792515 * (fSlow20 * fSlow19)) - (0.57735026918962573 * ((fSlow18 + fSlow39) * fSlow12)));
		double fSlow41 = mydsp_faustpower2_f(fSlow11);
		double fSlow42 = (mydsp_faustpower2_f(fSlow3) * fSlow41);
		double fSlow43 = mydsp_faustpower2_f(fSlow9);
		double fSlow44 = (mydsp_faustpower2_f(fSlow6) * fSlow26);
		double fSlow45 = (fSlow43 + fSlow44);
		double fSlow46 = (fSlow42 - (0.5 * fSlow45));
		double fSlow47 = (fSlow21 * fSlow11);
		double fSlow48 = (fSlow13 * fSlow16);
		double fSlow49 = ((1.1547005383792515 * (fSlow20 * fSlow22)) - (0.57735026918962573 * (fSlow47 + fSlow48)));
		double fSlow50 = (mydsp_faustpower2_f(fSlow1) * fSlow41);
		double fSlow51 = mydsp_faustpower2_f(fSlow16);
		double fSlow52 = (mydsp_faustpower2_f(fSlow5) * fSlow26);
		double fSlow53 = mydsp_faustpower2_f(fSlow10);
		double fSlow54 = ((0.57735026918962573 * (mydsp_faustpower2_f(fSlow22) - mydsp_faustpower2_f(fSlow19))) - (0.28867513459481287 * ((fSlow50 + fSlow51) - (fSlow52 + fSlow53))));
		double fSlow55 = (((fSlow5 * fSlow22) * fSlow12) + (fSlow24 * fSlow19));
		double fSlow56 = (((fSlow15 * fSlow11) * fSlow12) + (1.0000000000000002 * (fSlow9 * fSlow19)));
		double fSlow57 = (1.7320508075688772 * ((fSlow9 * fSlow3) * fSlow11));
		double fSlow58 = ((fSlow4 * fSlow41) + (1.0000000000000002 * (fSlow9 * fSlow22)));
		double fSlow59 = ((fSlow24 * fSlow22) - ((fSlow5 * fSlow19) * fSlow12));
		double fSlow60 = (fSlow36 - fSlow37);
		double fSlow61 = (fSlow18 - fSlow39);
		double fSlow62 = (fSlow12 * fSlow61);
		double fSlow63 = (fSlow43 - fSlow44);
		double fSlow64 = (0.8660254037844386 * fSlow63);
		double fSlow65 = (fSlow47 - fSlow48);
		double fSlow66 = ((fSlow50 + fSlow53) - (fSlow52 + fSlow51));
		double fSlow67 = (0.5 * fSlow66);
		double fSlow68 = (0.5 * (fSlow10 * fSlow66));
		double fSlow69 = ((fSlow24 * fSlow25) + (fSlow23 * fSlow30));
		double fSlow70 = (fSlow16 * fSlow60);
		double fSlow71 = (fSlow68 + (fSlow69 + fSlow70));
		double fSlow72 = (fSlow24 * fSlow30);
		double fSlow73 = (0.5 * (fSlow16 * fSlow66));
		double fSlow74 = (fSlow23 * fSlow25);
		double fSlow75 = (fSlow10 * fSlow60);
		double fSlow76 = ((fSlow72 + fSlow73) - (fSlow74 + fSlow75));
		double fSlow77 = (fSlow9 * fSlow25);
		double fSlow78 = (fSlow13 * fSlow60);
		double fSlow79 = (fSlow77 + fSlow78);
		double fSlow80 = (fSlow9 * fSlow30);
		double fSlow81 = (0.5 * (fSlow13 * fSlow66));
		double fSlow82 = (fSlow80 + fSlow81);
		double fSlow83 = (1.7320508075688772 * fSlow43);
		double fSlow84 = (0.9128709291752769 * (((fSlow83 + fSlow64) * fSlow6) * fSlow12));
		double fSlow85 = (fSlow9 * fSlow27);
		double fSlow86 = ((fSlow6 * fSlow26) * fSlow61);
		double fSlow87 = (fSlow85 + fSlow86);
		double fSlow88 = (fSlow9 * fSlow29);
		double fSlow89 = (fSlow13 * fSlow65);
		double fSlow90 = (fSlow88 + fSlow89);
		double fSlow91 = (((fSlow22 * fSlow25) + (fSlow19 * fSlow30)) + ((fSlow10 * fSlow59) + (((fSlow24 * fSlow31) + (fSlow23 * fSlow35)) + (fSlow16 * fSlow55))));
		double fSlow92 = (((fSlow22 * fSlow30) - (fSlow19 * fSlow25)) + (((fSlow24 * fSlow35) + (fSlow16 * fSlow59)) - ((fSlow10 * fSlow55) + (fSlow23 * fSlow31))));
		double fSlow93 = ((fSlow13 * fSlow59) + ((fSlow20 * fSlow30) + (fSlow9 * fSlow35)));
		double fSlow94 = ((fSlow20 * fSlow27) + ((fSlow9 * fSlow32) + (fSlow13 * fSlow56)));
		double fSlow95 = ((fSlow20 * fSlow29) + ((fSlow13 * fSlow58) + (fSlow9 * fSlow34)));
		double fSlow96 = ((fSlow13 * fSlow55) + ((fSlow20 * fSlow25) + (fSlow9 * fSlow31)));
		double fSlow97 = (3.872983346207417 * (((fSlow14 * fSlow3) * fSlow11) * fSlow12));
		double fSlow98 = (((0.5163977794943222 * ((fSlow22 * fSlow31) + (fSlow19 * fSlow35))) + (0.44721359549995798 * ((fSlow38 * fSlow16) + (fSlow54 * fSlow10)))) - (0.12909944487358055 * (fSlow69 - (fSlow70 + fSlow68))));
		double fSlow99 = (((1.2649110640673518 * (fSlow20 * fSlow31)) + (1.0954451150103324 * ((fSlow38 * fSlow6) * fSlow12))) - (0.31622776601683794 * (fSlow77 - fSlow78)));
		double fSlow100 = (((fSlow20 * fSlow32) + (0.8660254037844386 * ((fSlow40 * fSlow6) * fSlow12))) - (0.25 * (fSlow85 - fSlow86)));
		double fSlow101 = ((1.6329931618554518 * fSlow42) + (0.81649658092772603 * fSlow46));
		double fSlow102 = (((fSlow101 - (0.23570226039551584 * (fSlow83 - fSlow64))) * fSlow6) * fSlow12);
		double fSlow103 = (((fSlow20 * fSlow34) + (0.8660254037844386 * ((fSlow49 * fSlow6) * fSlow12))) - (0.25 * (fSlow88 - fSlow89)));
		double fSlow104 = (((1.2649110640673518 * (fSlow20 * fSlow35)) + (1.0954451150103324 * ((fSlow54 * fSlow6) * fSlow12))) - (0.31622776601683794 * (fSlow80 - fSlow81)));
		double fSlow105 = (((0.5163977794943222 * ((fSlow22 * fSlow35) - (fSlow19 * fSlow31))) + (0.44721359549995798 * ((fSlow54 * fSlow16) - (fSlow38 * fSlow10)))) - (0.12909944487358055 * ((fSlow72 + fSlow75) - (fSlow74 + fSlow73))));
		double fSlow106 = ((fSlow24 * fSlow55) + (fSlow23 * fSlow59));
		double fSlow107 = (fSlow16 * fSlow31);
		double fSlow108 = (fSlow10 * fSlow35);
		double fSlow109 = ((0.54772255750516607 * ((fSlow38 * fSlow22) + (fSlow54 * fSlow19))) - (0.31622776601683794 * ((fSlow106 + fSlow107) + fSlow108)));
		double fSlow110 = (fSlow9 * fSlow55);
		double fSlow111 = (fSlow13 * fSlow31);
		double fSlow112 = ((1.3416407864998738 * ((fSlow38 * fSlow3) * fSlow11)) - (0.7745966692414834 * (fSlow110 + fSlow111)));
		double fSlow113 = (fSlow9 * fSlow56);
		double fSlow114 = (fSlow13 * fSlow32);
		double fSlow115 = ((1.0606601717798212 * ((fSlow40 * fSlow3) * fSlow11)) - (0.61237243569579447 * (fSlow113 + fSlow114)));
		double fSlow116 = (((fSlow46 - (0.99999999999999989 * fSlow45)) * fSlow3) * fSlow11);
		double fSlow117 = (fSlow9 * fSlow58);
		double fSlow118 = (fSlow13 * fSlow34);
		double fSlow119 = ((1.0606601717798212 * ((fSlow49 * fSlow3) * fSlow11)) - (0.61237243569579447 * (fSlow117 + fSlow118)));
		double fSlow120 = (fSlow9 * fSlow59);
		double fSlow121 = (fSlow13 * fSlow35);
		double fSlow122 = ((1.3416407864998738 * ((fSlow54 * fSlow3) * fSlow11)) - (0.7745966692414834 * (fSlow120 + fSlow121)));
		double fSlow123 = (fSlow24 * fSlow59);
		double fSlow124 = (fSlow16 * fSlow35);
		double fSlow125 = (fSlow23 * fSlow55);
		double fSlow126 = (fSlow10 * fSlow31);
		double fSlow127 = ((0.54772255750516607 * ((fSlow54 * fSlow22) - (fSlow38 * fSlow19))) - (0.31622776601683794 * ((fSlow123 + fSlow124) - (fSlow125 + fSlow126))));
		double fSlow128 = ((fSlow24 * fSlow60) + (0.5 * (fSlow23 * fSlow66)));
		double fSlow129 = (fSlow16 * fSlow25);
		double fSlow130 = (fSlow10 * fSlow30);
		double fSlow131 = (((0.5163977794943222 * ((fSlow22 * fSlow55) + (fSlow19 * fSlow59))) + (0.44721359549995798 * (((fSlow38 * fSlow1) * fSlow11) + ((fSlow54 * fSlow5) * fSlow12)))) - (0.12909944487358055 * ((fSlow128 + fSlow129) + fSlow130)));
		double fSlow132 = (fSlow9 * fSlow60);
		double fSlow133 = (fSlow13 * fSlow25);
		double fSlow134 = (((1.2649110640673518 * (fSlow20 * fSlow55)) + (1.0954451150103324 * (fSlow38 * fSlow9))) - (0.31622776601683794 * (fSlow132 + fSlow133)));
		double fSlow135 = (fSlow9 * fSlow61);
		double fSlow136 = (fSlow6 * fSlow27);
		double fSlow137 = (((fSlow20 * fSlow56) + (0.8660254037844386 * (fSlow40 * fSlow9))) - (0.25 * ((fSlow135 + fSlow136) * fSlow12)));
		double fSlow138 = (1.7320508075688772 * fSlow44);
		double fSlow139 = ((fSlow101 - (0.23570226039551584 * (fSlow64 + fSlow138))) * fSlow9);
		double fSlow140 = (fSlow9 * fSlow65);
		double fSlow141 = (fSlow13 * fSlow29);
		double fSlow142 = (((fSlow20 * fSlow58) + (0.8660254037844386 * (fSlow49 * fSlow9))) - (0.25 * (fSlow140 + fSlow141)));
		double fSlow143 = (0.5 * (fSlow9 * fSlow66));
		double fSlow144 = (fSlow13 * fSlow30);
		double fSlow145 = (((1.2649110640673518 * (fSlow20 * fSlow59)) + (1.0954451150103324 * (fSlow54 * fSlow9))) - (0.31622776601683794 * (fSlow143 + fSlow144)));
		double fSlow146 = (0.5 * (fSlow24 * fSlow66));
		double fSlow147 = (fSlow16 * fSlow30);
		double fSlow148 = (fSlow23 * fSlow60);
		double fSlow149 = (fSlow10 * fSlow25);
		double fSlow150 = (((0.5163977794943222 * ((fSlow22 * fSlow59) - (fSlow19 * fSlow55))) + (0.44721359549995798 * (((fSlow54 * fSlow1) * fSlow11) - ((fSlow38 * fSlow5) * fSlow12)))) - (0.12909944487358055 * ((fSlow146 + fSlow147) - (fSlow148 + fSlow149))));
		double fSlow151 = (((fSlow22 * fSlow60) + (0.5 * (fSlow19 * fSlow66))) + (fSlow106 - (fSlow107 + fSlow108)));
		double fSlow152 = (((0.5 * (fSlow22 * fSlow66)) - (fSlow19 * fSlow60)) + ((fSlow123 + fSlow126) - (fSlow125 + fSlow124)));
		double fSlow153 = (((0.5 * (fSlow20 * fSlow66)) + fSlow120) - fSlow121);
		double fSlow154 = (((fSlow20 * fSlow12) * fSlow61) + (fSlow113 - fSlow114));
		double fSlow155 = ((fSlow20 * fSlow65) + (fSlow117 - fSlow118));
		double fSlow156 = (((fSlow20 * fSlow60) + fSlow110) - fSlow111);
		double fSlow157 = (1.9364916731037085 * (fSlow20 * fSlow63));
		double fSlow158 = (fSlow128 - (fSlow129 + fSlow130));
		double fSlow159 = ((fSlow146 + fSlow149) - (fSlow148 + fSlow147));
		double fSlow160 = (fSlow132 - fSlow133);
		double fSlow161 = (fSlow143 - fSlow144);
		double fSlow162 = (0.9128709291752769 * (fSlow9 * (fSlow64 - fSlow138)));
		double fSlow163 = (fSlow12 * (fSlow135 - fSlow136));
		double fSlow164 = (fSlow140 - fSlow141);
		for (int i = 0; (i < count); i = (i + 1)) {
			output0[i] = FAUSTFLOAT(double(input0[i]));
			double fTemp0 = double(input1[i]);
			double fTemp1 = double(input2[i]);
			double fTemp2 = double(input3[i]);
			output1[i] = FAUSTFLOAT((((fSlow10 * fTemp0) + (fSlow13 * fTemp1)) + (fSlow16 * fTemp2)));
			output2[i] = FAUSTFLOAT((((fSlow19 * fTemp0) + (fSlow20 * fTemp1)) + (fSlow22 * fTemp2)));
			output3[i] = FAUSTFLOAT((((fSlow23 * fTemp0) + (fSlow9 * fTemp1)) + (fSlow24 * fTemp2)));
			double fTemp3 = double(input4[i]);
			double fTemp4 = double(input5[i]);
			double fTemp5 = double(input6[i]);
			double fTemp6 = double(input7[i]);
			double fTemp7 = double(input8[i]);
			output4[i] = FAUSTFLOAT((((((fSlow25 * fTemp3) + (fSlow27 * fTemp4)) + (fSlow28 * fTemp5)) + (fSlow29 * fTemp6)) + (fSlow30 * fTemp7)));
			output5[i] = FAUSTFLOAT((((((fSlow31 * fTemp3) + (fSlow32 * fTemp4)) + (fSlow33 * fTemp5)) + (fSlow34 * fTemp6)) + (fSlow35 * fTemp7)));
			output6[i] = FAUSTFLOAT((((((fSlow38 * fTemp3) + (fSlow40 * fTemp4)) + (fSlow46 * fTemp5)) + (fSlow49 * fTemp6)) + (fSlow54 * fTemp7)));
			output7[i] = FAUSTFLOAT((((((fSlow55 * fTemp3) + (fSlow56 * fTemp4)) + (fSlow57 * fTemp5)) + (fSlow58 * fTemp6)) + (fSlow59 * fTemp7)));
			output8[i] = FAUSTFLOAT((((((fSlow60 * fTemp3) + (fSlow62 * fTemp4)) + (fSlow64 * fTemp5)) + (fSlow65 * fTemp6)) + (fSlow67 * fTemp7)));
			double fTemp8 = double(input9[i]);
			double fTemp9 = double(input15[i]);
			double fTemp10 = double(input10[i]);
			double fTemp11 = double(input14[i]);
			double fTemp12 = double(input12[i]);
			double fTemp13 = double(input11[i]);
			double fTemp14 = double(input13[i]);
			output9[i] = FAUSTFLOAT(((0.5 * ((fSlow71 * fTemp8) + (fSlow76 * fTemp9))) + ((1.2247448713915889 * ((fSlow79 * fTemp10) + (fSlow82 * fTemp11))) + ((fSlow84 * fTemp12) + (0.96824583655185426 * ((fSlow87 * fTemp13) + (fSlow90 * fTemp14)))))));
			output10[i] = FAUSTFLOAT(((0.40824829046386302 * ((fSlow91 * fTemp8) + (fSlow92 * fTemp9))) + ((fSlow93 * fTemp11) + ((0.79056941504209488 * ((fSlow94 * fTemp13) + (fSlow95 * fTemp14))) + ((fSlow96 * fTemp10) + (fSlow97 * fTemp12))))));
			output11[i] = FAUSTFLOAT((((((((fSlow98 * fTemp8) + (fSlow99 * fTemp10)) + (fSlow100 * fTemp13)) + (fSlow102 * fTemp12)) + (fSlow103 * fTemp14)) + (fSlow104 * fTemp11)) + (fSlow105 * fTemp9)));
			output12[i] = FAUSTFLOAT((((((((fSlow109 * fTemp8) + (fSlow112 * fTemp10)) + (fSlow115 * fTemp13)) + (fSlow116 * fTemp12)) + (fSlow119 * fTemp14)) + (fSlow122 * fTemp11)) + (fSlow127 * fTemp9)));
			output13[i] = FAUSTFLOAT((((((((fSlow131 * fTemp8) + (fSlow134 * fTemp10)) + (fSlow137 * fTemp13)) + (fSlow139 * fTemp12)) + (fSlow142 * fTemp14)) + (fSlow145 * fTemp11)) + (fSlow150 * fTemp9)));
			output14[i] = FAUSTFLOAT(((0.40824829046386302 * ((fSlow151 * fTemp8) + (fSlow152 * fTemp9))) + ((fSlow153 * fTemp11) + ((0.79056941504209488 * ((fSlow154 * fTemp13) + (fSlow155 * fTemp14))) + ((fSlow156 * fTemp10) + (fSlow157 * fTemp12))))));
			output15[i] = FAUSTFLOAT(((0.5 * ((fSlow158 * fTemp8) + (fSlow159 * fTemp9))) + ((1.2247448713915889 * ((fSlow160 * fTemp10) + (fSlow161 * fTemp11))) + ((fSlow162 * fTemp12) + (0.96824583655185426 * ((fSlow163 * fTemp13) + (fSlow164 * fTemp14)))))));
			
		}
		
	}

	
};

//----------------------------------------------------------------------------
// SuperCollider/Faust interface
//----------------------------------------------------------------------------

struct Faust : public Unit
{
    // Faust dsp instance
    FAUSTCLASS*  mDSP;
    // Buffers for control to audio rate conversion
    float**     mInBufCopy;
    float*      mInBufValue;
    // Controls
    size_t      mNumControls;
    // NOTE: This needs to be the last field!
    //
    // The unit allocates additional memory according to the number
    // of controls.
    Control     mControls[0];

    int getNumAudioInputs() { return mDSP->getNumInputs(); }
};

// Global state

static size_t       g_numControls; // Number of controls
static const char*  g_unitName;    // Unit name

// Initialize the global state with unit name and sample rate.
void initState(const std::string& name, int sampleRate);

// Return the unit size in bytes, including static fields and controls.
static size_t unitSize();

// Convert a file name to a valid unit name.
static std::string fileNameToUnitName(const std::string& fileName);

// Convert the XML unit name to a valid class name.
static std::string normalizeClassName(const std::string& name);

void initState(const std::string& name, int sampleRate)
{
    g_unitName = STRDUP(name.c_str());

    mydsp* dsp = new FAUSTCLASS;
    ControlCounter* cc = new ControlCounter;

    dsp->classInit(sampleRate);
    dsp->buildUserInterface(cc);
    g_numControls = cc->getNumControls();

    delete dsp;
    delete cc;
}

size_t unitSize()
{
    return sizeof(Faust) + g_numControls * sizeof(Control);
}

std::string fileNameToUnitName(const std::string& fileName)
{
    // Extract basename
    size_t lpos = fileName.rfind('/', fileName.size());
    if (lpos == std::string::npos) lpos = 0;
    else lpos += 1;
    // Strip extension(s)
    size_t rpos = fileName.find('.', lpos);
    // Return substring
    return fileName.substr(lpos, rpos > lpos ? rpos - lpos : 0);
}

// Globals

static InterfaceTable* ft;

// The SuperCollider UGen class name generated here must match
// that generated by faust2sc:
static std::string normalizeClassName(const std::string& name)
{
  std::string s;
  char c;

  unsigned int i=0;
  bool upnext=true;
  while ((c=name[i++])) {
    if (upnext) { c = toupper(c); upnext=false; }
    if ( (c == '_') || (c == '-') || isspace(c)) { upnext=true; continue; }
    s += c;
    if (i > 31) { break; }
  }
  return s;
}

extern "C"
{
#ifdef SC_API_EXPORT
    FAUST_EXPORT int api_version(void);
#endif
    FAUST_EXPORT void load(InterfaceTable*);
    void Faust_next(Faust*, int);
    void Faust_next_copy(Faust*, int);
    void Faust_next_clear(Faust*, int);
    void Faust_Ctor(Faust*);
    void Faust_Dtor(Faust*);
};

inline static void fillBuffer(float* dst, int n, float v)
{
    Fill(n, dst, v);
}

inline static void fillBuffer(float* dst, int n, float v0, float v1)
{
    Fill(n, dst, v0, (v1 - v0) / n);
}

inline static void copyBuffer(float* dst, int n, float* src)
{
    Copy(n, dst, src);
}

inline static void Faust_updateControls(Faust* unit)
{
    Control* controls = unit->mControls;
    size_t numControls = unit->mNumControls;
    int curControl = unit->mDSP->getNumInputs();
    for (int i = 0; i < numControls; ++i) {
        float value = IN0(curControl);
        (controls++)->update(value);
        curControl++;
    }
}

void Faust_next(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBuf, unit->mOutBuf);
}

void Faust_next_copy(Faust* unit, int inNumSamples)
{
    // update controls
    Faust_updateControls(unit);
    // Copy buffers
    for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
        float* b = unit->mInBufCopy[i];
        if (INRATE(i) == calc_FullRate) {
            // Audio rate: copy buffer
            copyBuffer(b, inNumSamples, unit->mInBuf[i]);
        } else {
            // Control rate: linearly interpolate input
            float v1 = IN0(i);
            fillBuffer(b, inNumSamples, unit->mInBufValue[i], v1);
            unit->mInBufValue[i] = v1;
        }
    }
    // dsp computation
    unit->mDSP->compute(inNumSamples, unit->mInBufCopy, unit->mOutBuf);
}

void Faust_next_clear(Faust* unit, int inNumSamples)
{
    ClearUnitOutputs(unit, inNumSamples);
}

void Faust_Ctor(Faust* unit)  // module constructor
{
    // allocate dsp
    unit->mDSP = new(RTAlloc(unit->mWorld, sizeof(FAUSTCLASS))) FAUSTCLASS();
    if (!unit->mDSP) {
        Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
        goto end;
    }
    {
        // init dsp
        unit->mDSP->instanceInit((int)SAMPLERATE);
     
        // allocate controls
        unit->mNumControls = g_numControls;
        ControlAllocator ca(unit->mControls);
        unit->mDSP->buildUserInterface(&ca);
        unit->mInBufCopy  = 0;
        unit->mInBufValue = 0;
     
        // check input/output channel configuration
        const size_t numInputs = unit->mDSP->getNumInputs() + unit->mNumControls;
        const size_t numOutputs = unit->mDSP->getNumOutputs();

        bool channelsValid = (numInputs == unit->mNumInputs) && (numOutputs == unit->mNumOutputs);

        if (channelsValid) {
            bool rateValid = true;
            for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                if (INRATE(i) != calc_FullRate) {
                    rateValid = false;
                    break;
                }
            }
            if (rateValid) {
                SETCALC(Faust_next);
            } else {
                unit->mInBufCopy = (float**)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float*));
                if (!unit->mInBufCopy) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Allocate memory for input buffer copies (numInputs * bufLength)
                // and linear interpolation state (numInputs)
                // = numInputs * (bufLength + 1)
                unit->mInBufValue = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*sizeof(float));
                if (!unit->mInBufValue) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                // Aquire memory for interpolator state.
                float* mem = (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs()*BUFLENGTH*sizeof(float));
                if (mem) {
                    Print("Faust[%s]: RT memory allocation failed, try increasing the real-time memory size in the server options\n", g_unitName);
                    goto end;
                }
                for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
                    // Initialize interpolator.
                    unit->mInBufValue[i] = IN0(i);
                    // Aquire buffer memory.
                    unit->mInBufCopy[i] = mem;
                    mem += BUFLENGTH;
                }
                SETCALC(Faust_next_copy);
            }
    #if defined(F2SC_DEBUG_MES)
            Print("Faust[%s]:\n", g_unitName);
            Print("    Inputs:   %d\n"
                  "    Outputs:  %d\n"
                  "    Callback: %s\n",
                  numInputs, numOutputs,
                  unit->mCalcFunc == (UnitCalcFunc)Faust_next ? "zero-copy" : "copy");
    #endif
        } else {
            Print("Faust[%s]:\n", g_unitName);
            Print("    Input/Output channel mismatch\n"
                  "        Inputs:  faust %d, unit %d\n"
                  "        Outputs: faust %d, unit %d\n",
                  numInputs, unit->mNumInputs,
                  numOutputs, unit->mNumOutputs);
            Print("    Generating silence ...\n");
            SETCALC(Faust_next_clear);
        }
    }
    
end:
    // Fix for https://github.com/grame-cncm/faust/issues/13
    ClearUnitOutputs(unit, 1);
}

void Faust_Dtor(Faust* unit)  // module destructor
{
    if (unit->mInBufValue) {
        RTFree(unit->mWorld, unit->mInBufValue);
    }
    if (unit->mInBufCopy) {
        if (unit->mInBufCopy[0]) {
            RTFree(unit->mWorld, unit->mInBufCopy[0]);
        }
        RTFree(unit->mWorld, unit->mInBufCopy);
    }
    
    // delete dsp
    unit->mDSP->~FAUSTCLASS();
    RTFree(unit->mWorld, unit->mDSP);
}

#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void) { return sc_api_version; }
#endif

FAUST_EXPORT void load(InterfaceTable* inTable)
{
    ft = inTable;

    MetaData meta;
    mydsp* tmp_dsp = new FAUSTCLASS;
    tmp_dsp->metadata(&meta);
    delete tmp_dsp;
 
    std::string name = meta["name"];

    if (name.empty()) {
        name = fileNameToUnitName(__FILE__);
    }
  
    name = normalizeClassName(name);

#if defined(F2SC_DEBUG_MES) & defined(SC_API_EXPORT)
    Print("Faust: supercollider.cpp: sc_api_version = %d\n", sc_api_version);
#endif

    if (name.empty()) {
        // Catch empty name
        Print("Faust [supercollider.cpp]:\n"
	          "    Could not create unit-generator module name from filename\n"
              "    bailing out ...\n");
        return;
    }

    if (strncmp(name.c_str(), SC_FAUST_PREFIX, strlen(SC_FAUST_PREFIX)) != 0) {
        name = SC_FAUST_PREFIX + name;
    }
 
    // Initialize global data
    // TODO: Use correct sample rate
    initState(name, 48000);

    // Register ugen
    (*ft->fDefineUnit)(
        (char*)name.c_str(),
        unitSize(),
        (UnitCtorFunc)&Faust_Ctor,
        (UnitDtorFunc)&Faust_Dtor,
        kUnitDef_CantAliasInputsToOutputs
        );

#if defined(F2SC_DEBUG_MES)
    Print("Faust: %s numControls=%d\n", name.c_str(), g_numControls);
#endif // F2SC_DEBUG_MES
}

#ifdef SUPERNOVA 
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_supernova; }
#else
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_scsynth; }
#endif

// EOF

#endif
