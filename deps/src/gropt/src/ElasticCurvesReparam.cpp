
#include "ElasticCurvesReparam.h"

std::map<integer *, integer> *CheckMemoryDeleted;

void optimum_reparam(double *C1, double *C2, int n, int d, double w,
                          bool onlyDP, char *methodname, double *opt, bool swap,
                          double *fopts, double *comtime)
{
	/* dimensions of input matrices */
    /* opt size is n + d*d +1 */
    /* fopts and comtime are 5 x 1*/
	integer n1, d1;
    n1 = static_cast<integer> (n);
    d1 = static_cast<integer> (d);
    bool rotated = false;
    bool isclosed = false;
    bool swapi;

	init_genrand(0);

	CheckMemoryDeleted = new std::map<integer *, integer>;

	integer numofmanis = 3;
	integer numofmani1 = 1;
	integer numofmani2 = 1;
	integer numofmani3 = 1;
	L2SphereVariable FNSV(n);
	OrthGroupVariable OGV(d);
	EucVariable EucV(1);
	ProductElement *Xopt = new ProductElement(numofmanis, &FNSV, numofmani1, &OGV, numofmani2, &EucV, numofmani3);

	integer ns, lms;

	DriverElasticCurvesRO(C1, C2, d1, n1, w, rotated, isclosed, onlyDP, 0, methodname,
		                  1, Xopt, swapi, fopts, comtime, ns, lms);

    swap = swapi;

	/* get output data */
    integer sizex = n1 + d1 * d1 + 1;
    const double *Xoptptr = Xopt->ObtainReadData();
	integer inc = 1;
	dcopy_(&sizex, const_cast<double *> (Xoptptr), &inc, opt, &inc);

	delete Xopt;

	std::map<integer *, integer>::iterator iter = CheckMemoryDeleted->begin();
	for (iter = CheckMemoryDeleted->begin(); iter != CheckMemoryDeleted->end(); iter++)
	{
		if (iter->second != 1)
			std::cout << "Global address:" << iter->first << ", sharedtimes:" << iter->second << std::endl;
	}
	delete CheckMemoryDeleted;
	return;
}

