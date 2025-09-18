CXX           = icpc
CC            = icpc
CXXFLAGS      = -O3 \
                -DNDEBUG \
                -D_LINUX \
              	-diag-disable=10441
DEST          = ./
LIBS          = 
OBJS          = main.o \
                MeshData.o \
                MeshDataNonConformingHexaElement.o \
                ResistivityBlock.o \
                Util.o
PROGRAM       = FemticDHexaMesh2XYZformat

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:;		rm -f *.o *~ $(PROGRAM)
