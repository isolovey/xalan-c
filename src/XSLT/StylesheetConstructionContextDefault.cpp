/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

#include "StylesheetConstructionContextDefault.hpp"



#include <algorithm>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/URISupport.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include "ElemTemplateElement.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTInputSource.hpp"



StylesheetConstructionContextDefault::StylesheetConstructionContextDefault(
			XSLTEngineImpl&				processor,
			XPathFactory&				xpathFactory,
			VectorAllocatorSizeType		theAllocatorSize) :
	StylesheetConstructionContext(),
	m_processor(processor),
	m_xpathFactory(xpathFactory),
	m_xpathProcessor(new XPathProcessorImpl),
	m_stylesheets(),
	m_stringPool(),
	m_xalanDOMCharVectorAllocator(theAllocatorSize),
	m_tempBuffer()
{
}



StylesheetConstructionContextDefault::~StylesheetConstructionContextDefault()
{
	reset();
}



void
StylesheetConstructionContextDefault::error(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.error(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.error(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.error(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::error(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::warn(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.warn(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.warn(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.warn(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::warn(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::warn(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::message(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.message(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.message(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.message(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::message(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::message(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_stylesheets.begin(),
			 m_stylesheets.end(),
			 DeleteFunctor<StylesheetRoot>());

	m_stylesheets.clear();

	m_xpathFactory.reset();
}



StylesheetRoot*
StylesheetConstructionContextDefault::create(const XalanDOMString&	theBaseIdentifier)
{
	StylesheetRoot* const	theStylesheetRoot =
		new StylesheetRoot(theBaseIdentifier, *this);

	m_stylesheets.insert(theStylesheetRoot);

	return theStylesheetRoot;
}



StylesheetRoot*
StylesheetConstructionContextDefault::create(const XSLTInputSource&		theInputSource)
{
	const XMLCh* const	theSystemID =
				theInputSource.getSystemId();

	const XalanDOMString	theBaseIdentifier =
				theSystemID == 0 ? XalanDOMString() :
				XalanDOMString(theSystemID);

	return create(theBaseIdentifier);
}



Stylesheet*
StylesheetConstructionContextDefault::create(
			StylesheetRoot&			theStylesheetRoot,
			const XalanDOMString&	theBaseIdentifier)
{
	Stylesheet* const	theStylesheet =
		new Stylesheet(
			theStylesheetRoot,
			theBaseIdentifier,
			*this);

	return theStylesheet;
}



void
StylesheetConstructionContextDefault::destroy(StylesheetRoot*	theStylesheetRoot)
{
	const StylesheetSetType::iterator	i =
		m_stylesheets.find(theStylesheetRoot);

	if (i != m_stylesheets.end())
	{
		m_stylesheets.erase(i);

		delete theStylesheetRoot;
	}
}



StylesheetConstructionContextDefault::URLAutoPtrType
StylesheetConstructionContextDefault::getURLFromString(const XalanDOMString&	urlString)
{
	return URISupport::getURLFromString(urlString);
}



XalanDOMString
StylesheetConstructionContextDefault::getURLStringFromString(const XalanDOMString&	urlString)
{
	return URISupport::getURLStringFromString(urlString);
}



StylesheetConstructionContextDefault::URLAutoPtrType
StylesheetConstructionContextDefault::getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return URISupport::getURLFromString(urlString, base);
}



XalanDOMString
StylesheetConstructionContextDefault::getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return URISupport::getURLStringFromString(urlString, base);
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXSLTNamespaceURI() const
{
	return XSLTEngineImpl::getXSLNameSpaceURL();
}



XPath*
StylesheetConstructionContextDefault::createMatchPattern(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	// Note that we use the current locator from the
	// processing stack, and not the locator passed in.
	// This is because the locator on the stack is active,
	// during construction, while the locator passed in
	// will be used at run-time.
	m_xpathProcessor->initMatchPattern(
			*xpath,
			str,
			resolver,
			getLocatorFromStack());

	xpath->setInStylesheet(true);

	xpath->setLocator(locator);

	return xpath;
}



XPath*
StylesheetConstructionContextDefault::createMatchPattern(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver)
{
	assert(str != 0);

	assign(m_tempBuffer, str);

	return createMatchPattern(locator, m_tempBuffer, resolver);
}



XPath*
StylesheetConstructionContextDefault::createXPath(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	// Note that we use the current locator from the
	// processing stack, and not the locator passed in.
	// This is because the locator on the stack is active,
	// during construction, while the locator passed in
	// will be used at run-time.
	m_xpathProcessor->initXPath(
			*xpath,
			str,
			resolver,
			getLocatorFromStack());

	xpath->setInStylesheet(true);

	xpath->setLocator(locator);

	return xpath;
}



XPath*
StylesheetConstructionContextDefault::createXPath(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver)
{
	assert(str != 0);

	assign(m_tempBuffer, str);

	return createXPath(locator, m_tempBuffer, resolver);
}



const Locator*
StylesheetConstructionContextDefault::getLocatorFromStack() const
{
	return m_processor.getLocatorFromStack();
}



void
StylesheetConstructionContextDefault::pushLocatorOnStack(const Locator*		locator)
{
	m_processor.pushLocatorOnStack(locator);
}



void
StylesheetConstructionContextDefault::popLocatorStack()
{
	m_processor.popLocatorStack();
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXalanXSLNameSpaceURL() const
{
	return XSLTEngineImpl::getXalanXSLNameSpaceURL();
}



XalanDocument*
StylesheetConstructionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler, 
			XalanDocument*			docToRegister)
{
	return m_processor.parseXML(urlString, docHandler, docToRegister);
}



int
StylesheetConstructionContextDefault::getElementToken(const XalanDOMString&		name)
{
	return StylesheetConstructionContextDefault::getElementToken(name);
}



int
StylesheetConstructionContextDefault::getElementToken(const XalanDOMString&		name)
{
	// Find the entity, if any...
	const ElementTokenTableEntry*	theFirst = s_elementTokenTable;
	const ElementTokenTableEntry*	theLast = &s_elementTokenTableLast;

	while(theFirst <= theLast)
	{
		const ElementTokenTableEntry* const	
			theCurrent = theFirst + (theLast - theFirst) / 2;

		const int	theResult = compareIgnoreCaseASCII(name, theCurrent->m_name);

		if (theResult < 0)
		{
			theLast = theCurrent - 1;
		}
		else if (theResult > 0)
		{
			theFirst = theCurrent + 1;
		}
		else
		{
			return theCurrent->m_token;
		}
	}

	return s_elementTokenTableDummy.m_token;
}



double
StylesheetConstructionContextDefault::getXSLTVersionSupported() const
{
	return XSLTEngineImpl::getXSLTVerSupported();
}



const XalanDOMString&
StylesheetConstructionContextDefault::getPooledString(const XalanDOMString&		theString)
{
	return m_stringPool.get(theString);
}



const XalanDOMString&
StylesheetConstructionContextDefault::getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	return m_stringPool.get(theString, theLength);
}



XalanDOMChar*
StylesheetConstructionContextDefault::allocateVector(XalanDOMString::size_type	theLength)
{
	return m_xalanDOMCharVectorAllocator.allocate(theLength);
}



XalanDOMChar*
StylesheetConstructionContextDefault::allocateVector(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength,
			bool						fTerminate)
{
	assert(theString != 0);

	const XalanDOMString::size_type		theActualLength =
		theLength == XalanDOMString::npos ? XalanDOMString::length(theString) : theLength;

	XalanDOMChar*	theVector =
		m_xalanDOMCharVectorAllocator.allocate(fTerminate == true ? theActualLength + 1 : theActualLength);

	XalanDOMChar* const		theEnd =
		XalanCopy(theString, theString + theActualLength, theVector);

	if (fTerminate == true)
	{
		*theEnd = XalanDOMChar(0);
	}

	return theVector;
}



const XalanDOMChar	StylesheetConstructionContextDefault::s_if[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_key[] =
{
	XalanUnicode::charLetter_k,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_copy[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_sort[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_text[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_when[] =
{
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_empty[] =
{
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_param[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_choose[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_import[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_number[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_output[] =
{
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_comment[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_copyOf[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_y,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_element[] =
{
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_include[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_message[] =
{
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_fallback[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_k,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_forEach[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_template[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_valueOf[] =
{
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_variable[] =
{
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_attribute[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_otherwise[] =
{
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_transform[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_m,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_stylesheet[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_withParam[] =
{
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_stripSpace[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_p,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_applyImports[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_y,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_s,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_attributeSet[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_callTemplate[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_l,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_decimalFormat[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_preserveSpace[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_applyTemplates[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_y,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_namespaceAlias[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_s,
	0
};

const XalanDOMChar	StylesheetConstructionContextDefault::s_processingInstruction[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};


const StylesheetConstructionContextDefault::ElementTokenTableEntry	StylesheetConstructionContextDefault::s_elementTokenTable[] =
{
	{
		s_if,
		ELEMNAME_IF
	},
	{
		s_key,
		ELEMNAME_KEY
	},
	{
		s_copy,
		ELEMNAME_COPY
	},
	{
		s_sort,
		ELEMNAME_SORT
	},
	{
		s_text,
		ELEMNAME_TEXT
	},
	{
		s_when,
		ELEMNAME_WHEN
	},
	{
		s_empty,
		ELEMNAME_EMPTY
	},
	{
		s_param,
		ELEMNAME_PARAM
	},
	{
		s_choose,
		ELEMNAME_CHOOSE
	},
	{
		s_import,
		ELEMNAME_IMPORT
	},
	{
		s_number,
		ELEMNAME_NUMBER
	},
	{
		s_output,
		ELEMNAME_OUTPUT
	},
	{
		s_comment,
		ELEMNAME_COMMENT
	},
	{
		s_copyOf,
		ELEMNAME_COPY_OF
	},
	{
		s_element,
		ELEMNAME_ELEMENT
	},
	{
		s_include,
		ELEMNAME_INCLUDE
	},
	{
		s_message,
		ELEMNAME_MESSAGE
	},
	{
		s_fallback,
		ELEMNAME_FALLBACK
	},
	{
		s_forEach,
		ELEMNAME_FOR_EACH
	},
	{
		s_template,
		ELEMNAME_TEMPLATE
	},
	{
		s_valueOf,
		ELEMNAME_VALUE_OF
	},
	{
		s_variable,
		ELEMNAME_VARIABLE
	},
	{
		s_attribute,
		ELEMNAME_ATTRIBUTE
	},
	{
		s_otherwise,
		ELEMNAME_OTHERWISE
	},
	{
		s_transform,
		ELEMNAME_STYLESHEET
	},
	{
		s_stylesheet,
		ELEMNAME_STYLESHEET
	},
	{
		s_withParam,
		ELEMNAME_WITH_PARAM
	},
	{
		s_stripSpace,
		ELEMNAME_STRIP_SPACE
	},
	{
		s_applyImports,
		ELEMNAME_APPLY_IMPORTS
	},
	{
		s_attributeSet,
		ELEMNAME_ATTRIBUTE_SET
	},
	{
		s_callTemplate,
		ELEMNAME_CALL_TEMPLATE
	},
	{
		s_decimalFormat,
		ELEMNAME_DECIMAL_FORMAT
	},
	{
		s_preserveSpace,
		ELEMNAME_PRESERVE_SPACE
	},
	{
		s_applyTemplates,
		ELEMNAME_APPLY_TEMPLATES
	},
	{
		s_namespaceAlias,
		ELEMNAME_NAMESPACE_ALIAS
	},
	{
		s_processingInstruction,
		ELEMNAME_PI
	},
	// This must be the last one...
	{
		0,
		ELEMNAME_UNDEFINED
	}
};

const unsigned int	StylesheetConstructionContextDefault::s_elementTokenTableSize =
		sizeof(s_elementTokenTable) / sizeof(s_elementTokenTable[0]);

const StylesheetConstructionContextDefault::ElementTokenTableEntry&		StylesheetConstructionContextDefault::s_elementTokenTableLast =
	StylesheetConstructionContextDefault::s_elementTokenTable[s_elementTokenTableSize - 2];

const StylesheetConstructionContextDefault::ElementTokenTableEntry&		StylesheetConstructionContextDefault::s_elementTokenTableDummy =
	StylesheetConstructionContextDefault::s_elementTokenTable[s_elementTokenTableSize - 1];
