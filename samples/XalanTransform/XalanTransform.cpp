


#include <XalanTransformer/XalanTransformer.hpp>



#include <strstream>



int
main(
			int				 argc,
			const char*		 argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
	using std::cout;
	using std::cerr;
    using std::endl;
#endif

    if (argc < 3 || argc > 4)
	{
		cerr << "Usage: XalanTransform XMLFileName XSLFileName [OutFileName]" << endl;
		return -1;
	}

    // Initialize Xalan.
    XalanTransformer::initialize();

    // Create a XalanTransformer.
	XalanTransformer theXalanTransformer;

	const char* 	theXMLFileName = argv[1];
	const char* 	theXSLFileName = argv[2];

    int				theResult = 0;

    if (argc == 4)
	{
	    const char* 		theOutFileName = argv[3];

        // Do the transform.
        theResult = theXalanTransformer.transform(theXMLFileName, theXSLFileName, theOutFileName);
        
        if(theResult != 0)
	    {
		    cerr << "XalanError: \n" << theXalanTransformer.getLastError();
	    }
    }
    else
    {
        ostrstream	theOutput;

        // Do the transform.
        theResult = theXalanTransformer.transform(theXMLFileName, theXSLFileName, theOutput);
        
        if(theResult != 0)
	    {
		    cerr << "XalanError: \n" << theXalanTransformer.getLastError();
	    }
	    else
	    {
		    theOutput << '\0';

		    cout << theOutput.str();
	    }
    }

    // Terminate Xalan.
	XalanTransformer::terminate();

	return theResult;
}