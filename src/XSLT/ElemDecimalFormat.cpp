/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#include "ElemDecimalFormat.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"



ElemDecimalFormat::ElemDecimalFormat(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet& 					stylesheetTree,
			const AttributeList&			atts,
			int 							lineNumber,
			int 							columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_DECIMAL_FORMAT),
	m_countMatchPattern(0),
	m_fromMatchPattern(0),
	m_valueExpr(0),
	m_qname(),
	m_decimalFormatSymbols()
{
	m_decimalFormatSymbols.setInfinity(XalanDOMString());
	m_decimalFormatSymbols.setNaN(XalanDOMString());

	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar* const		aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_NAME))
		{
			assert(atts.getValue(i) != 0);

			m_qname.set(atts.getValue(i), getStylesheet().getNamespaces());
		}
		else if(equals(aname, Constants::ATTRNAME_DECIMALSEPARATOR))
		{
			const XalanDOMChar* const	decimalSepValue = atts.getValue(i);
			assert(decimalSepValue != 0);

			if (length(decimalSepValue) == 1)
			{
				m_decimalFormatSymbols.setDecimalSeparator(decimalSepValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal decimal-separator attribute value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_GROUPINGSEPARATOR))
		{
			const XalanDOMChar* const	sepValue = atts.getValue(i);
			assert(sepValue!= 0);

			if (length(sepValue) == 1)
			{
				m_decimalFormatSymbols.setGroupingSeparator(sepValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'grouping-separator' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_INFINITY))
		{
			assert(atts.getValue(i) != 0);

			m_decimalFormatSymbols.setInfinity(atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_MINUSSIGN))
		{
			const XalanDOMChar* const	minusValue = atts.getValue(i);
			assert(minusValue != 0);

			if (length(minusValue) == 1)
			{
				m_decimalFormatSymbols.setMinusSign(minusValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'minus-sign' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_NAN))
		{
			assert(atts.getValue(i) != 0);

			m_decimalFormatSymbols.setNaN(atts.getValue(i));
		}
		else if(equals(aname, Constants::ATTRNAME_PERCENT))
		{
			const XalanDOMChar* const	percentValue = atts.getValue(i);
			assert(percentValue != 0);

			if (length(percentValue) == 1)
			{
				m_decimalFormatSymbols.setPercent(percentValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'percent' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_PERMILLE))
		{
			const XalanDOMChar* const	permilleValue = atts.getValue(i);
			assert(permilleValue != 0);

			if (length(permilleValue) == 1)
			{
				m_decimalFormatSymbols.setPerMill(permilleValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'per-mille' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_ZERODIGIT))
		{
			const XalanDOMChar* const	zeroDigitValue = atts.getValue(i);
			assert(zeroDigitValue != 0);

			if (length(zeroDigitValue) == 1)
			{
				m_decimalFormatSymbols.setZeroDigit(zeroDigitValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'zero-digit' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_DIGIT))
		{
			const XalanDOMChar* const	digitValue = atts.getValue(i);
			assert(digitValue != 0);

			if (length(digitValue) == 1)
			{
				m_decimalFormatSymbols.setDigit(digitValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'digit' value",
					0,
					this);
			}
		}
		else if(equals(aname, Constants::ATTRNAME_PATTERNSEPARATOR))
		{
			const XalanDOMChar* const	patternSepValue = atts.getValue(i);
			assert(patternSepValue != 0);

			if (length(patternSepValue) == 1)
			{
				m_decimalFormatSymbols.setPatternSeparator(patternSepValue[0]);
			}
			else
			{
				constructionContext.warn(
					"xsl:decimal-format has an illegal 'pattern-separator' value",
					0,
					this);
			}
		}
		else if(!isAttrOK(aname, atts, i, constructionContext))
		{
			constructionContext.error(
				"xsl:decimal-format has an illegal attribute",
				0,
				this);
		}
	}

	if (length(m_decimalFormatSymbols.getInfinity()) == 0)
	{
		m_decimalFormatSymbols.setInfinity(Constants::ATTRVAL_INFINITY);
	}

	if (length(m_decimalFormatSymbols.getNaN()) == 0)
	{
		m_decimalFormatSymbols.setNaN(Constants::ATTRVAL_NAN);
	}

	// Look for duplicate decimal-format names
	const XalanDecimalFormatSymbols* const	theOther =
			stylesheetTree.getDecimalFormatSymbols(m_qname);

	if (theOther != 0 && *theOther != m_decimalFormatSymbols)
	{
		constructionContext.error(
			"Duplicate definition of xsl:decimal-format element",
			0,
			this);
	}
}



ElemDecimalFormat::~ElemDecimalFormat()
{
}



const XalanDOMString&
ElemDecimalFormat::getElementName() const
{
	return Constants::ELEMNAME_DECIMALFORMAT_WITH_PREFIX_STRING;
}



void
ElemDecimalFormat::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);
}
