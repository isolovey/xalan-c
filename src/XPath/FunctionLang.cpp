/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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
 */
#include "FunctionLang.hpp"



#include <DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionLang::FunctionLang() :
	m_attributeName(XALAN_STATIC_UCODE_STRING("lang"))
{
}



FunctionLang::~FunctionLang()
{
}



XObjectPtr
FunctionLang::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false);	

	const XalanNode*		parent = context;

	bool					fMatch = false;

	const XalanDOMString&	lang = arg1->str();

	while(0 != parent)
	{
		if(XalanNode::ELEMENT_NODE == parent->getNodeType())
		{
			const XalanElement* const	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanElement*)parent;
#else
				static_cast<const XalanElement*>(parent);
#endif

			const XalanDOMString		langVal =
				theElementNode->getAttributeNS(DOMServices::s_XMLNamespaceURI, m_attributeName);

			if(0 != length(langVal))
			{
				if(startsWith(toLowerCaseASCII(langVal), toLowerCaseASCII(lang)))
				{
					const XalanDOMString::size_type		valLen = length(lang);

					if(length(langVal) == valLen ||
					   charAt(langVal, valLen) == XalanUnicode::charHyphenMinus)
					{
						fMatch = true;

						break;
					}
				}
			}
		}

		parent = DOMServices::getParentOfNode(*parent);
	}

	return executionContext.getXObjectFactory().createBoolean(fMatch);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionLang*
#endif
FunctionLang::clone() const
{
	return new FunctionLang(*this);
}



const XalanDOMString
FunctionLang::getError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The lang() function takes one argument!"));
}



XALAN_CPP_NAMESPACE_END
