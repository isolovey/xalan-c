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
#if !defined(QNAMEBYVALUE_HEADER_GUARD_1357924680)
#define QNAMEBYVALUE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



// Base class header file...
#include <XPath/QName.hpp>



class XALAN_XPATH_EXPORT QNameByValue : public QName
{
public:

	/**
	 * Construct an empty QNameByValue.
	 */
	explicit
	QNameByValue();

	/**
	 * Copy constructor.
	 *
	 * @param theSource The source of the copy.
	 */
	QNameByValue(const QNameByValue&	theSource);

	/**
	 * Construct an instance from another QName.
	 *
	 * @param theSource The source of the copy.
	 */
	QNameByValue(const QName&	theSource);

	/**
	 * Construct a QNameByValue, with the supplied namespace and local part.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 */
	QNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart);

	/**
	 * Construct a QNameByValue from a string, resolving the prefix using the given
	 * namespace vector stack. The default namespace is not resolved.
	 *
	 * @param qname      QName string
	 * @param namespaces namespace vector stack to use
	 */
	QNameByValue(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces);

	/**
	 * Construct a QNameByValue from a string, resolving the prefix using the given
	 * namespace vector stack. The default namespace is not resolved.
	 *
	 * @param qname      QName string
	 * @param namespaces namespace vector stack to use
	 */
	QNameByValue(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces);

	/**
	 * Construct a QNameByValue from a string, resolving the prefix using the given
	 * namespace context. The default namespace is not resolved.
	 *
	 * @param qname            QName string
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport       XPath environment support class instance
	 * @param support          XPath support class instance
	 */
	QNameByValue(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const XPathSupport&		support);

	/**
	 * Construct a QNameByValue from a string, resolving the prefix using the given
	 * prefix resolver. The default namespace is not resolved.
	 *
	 * @param qname            QName string
	 * @param theResolver prefix resolver to use
	 */
	QNameByValue(
			const XalanDOMString&	qname,
			const PrefixResolver&	theResolver);

	virtual
	~QNameByValue();

	/**
	 * Retrieve the local part of qualified name.
	 * 
	 * @return local part string
	 */
	virtual const XalanDOMString&
	getLocalPart() const;

	/**
	 * Retrieve the namespace of qualified name.
	 * 
	 * @return namespace string
	 */
	virtual const XalanDOMString&
	getNamespace() const;

private:

	void
	initialize(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces);

	void
	resolvePrefix(
			const XalanDOMString&	qname,
			const PrefixResolver&	theResolver);

	XalanDOMString	m_namespace;

	XalanDOMString	m_localpart;
};



#endif	// QNAMEBYVALUE_HEADER_GUARD_1357924680
