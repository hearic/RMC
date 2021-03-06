/////////////////////////////////////////////////////////////////
////////////////  Key word list  2014-03-14  ////////////////////
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
/////////////////////// Geometry key words //////////////////////
/////////////////////////////////////////////////////////////////
// UNIVERSE
    // lat
    // move
    // rotate
    // pitch
    // scope
    // sita
	// cell
	    // mat
        // tmp
        // vol
        // void
        // fill
        // inner
// SURFACE
    // p
	    // bc
	// px
	// py
	// pz
	// so
	// s
	// sx
	// sy
	// sz
	// c/x
	// c/y
	// c/z
	// cx
	// cy
	// cz
	// k/x
	// k/y
	// k/z
	// kx
	// ky
	// kz
	// sq
	// gq
	// tx
	// ty
	// tz

/////////////////////////////////////////////////////////////////
/////////////////////// material key words //////////////////////	 
/////////////////////////////////////////////////////////////////
// MATERIAL
    // mat
	// sab
	// ceace
	// mgace

///////////////////////////////////////////////////////////////// 
/////////////////////// criticality key words /////////////////////
/////////////////////////////////////////////////////////////////
// CRITICALITY
    // PowerIter
        // population
    	// keff0
		// Batch
    // InitSrc
        // point
        // slab
        // sph
        // cyl/x
		// cyl/y
		// cyl/z    
    // RNG
       // type 
       // seed 
       // stride
    // ParallelBank

/////////////////////////////////////////////////////////////////
///////////////////////  Tally key words   //////////////////////
/////////////////////////////////////////////////////////////////
// TALLY
    // celltally
        // type
        // energy
		// filter
		// integral
		// flag
		// cell
    // meshtally
        // type
        // energy
		// scope
		// bound
	// cstally
        // cell
        // mat
		// mt
    // accetally
	    // map
		// union

/////////////////////////////////////////////////////////////////
/////////////// Source convergence key words ////////////////////
/////////////////////////////////////////////////////////////////
// CONVERGENCE
    // SEmesh
	    // scope
		// bound
	// FMmesh
	    // scope
		// bound
    // NPMmesh
        // scope
		// bound
    // AcceFsc
		// method
		// autofactor
		// factor
		// Wielandt
		// Superhistory
		// FissionMatrix
		
/////////////////////////////////////////////////////////////////		
//////////////////////// Burnup key words ///////////////////////
/////////////////////////////////////////////////////////////////
// BURNUP
	// BurnCell
	// Timestep
	// Power
	// AceLib
    // DepthLib
	// Substep
	// Inherent
	// Strategy
	// Solver
	// Parallel
	// Outputcell
	// Impnuc
	// XeEq
    // NAECF

/////////////////////////////////////////////////////////////////		
///////////////////////// Print key words ///////////////////////
/////////////////////////////////////////////////////////////////
// PRINT
	// mat
	// keff
	// source
	// celltally
	// meshtally
	// cstally
    // cyctally
    // inpfile

/////////////////////////////////////////////////////////////////		
///////////////////////// Plot key words ///////////////////////
/////////////////////////////////////////////////////////////////
// Plot
	// Continue-calculation = 0 / 1
	// ColorScheme = int (>0)
	// PlotID
        // type = slice / box
	    // color = mat / cell / surf / matsurf / cellsurf
		// pixels = 2 double - slice, 3 double - box
		// vertexes = 6 / 9 double - slice, 12 double - box


/////////////////////////////////////////////////////////////////		
///////////////////////// DEPTH key words ///////////////////////
/////////////////////////////////////////////////////////////////
// Depletion
		// Convertlib 1 decay.data xs.data yield.data DepthMainLIB   // 数据库转换选项： 将ORIGEN数据库转换为DEPTH数据库
		// Convertlib 2 tape9.inp DepthMainLIB
		// Decay / Flux / Power
        // Density
        // Solver   1: TTA   2: CRAM  3: QRAM  4：LPAM
        // TTA
        // CRAM
        // QRAM
        // LPAM
        // Print
        // Radioactivity
        // AbsorpRate
	    // FissionRate
	    // DecayHeat

