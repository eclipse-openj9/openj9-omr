Contributing {#Contributing}
============ 

All the documentation here is created through [Doxygen][dox]. 

    /**
     * This is a Javadoc style document, documenting class foo.
     * Note that the opening comment has two stars, rather than just one.
     */
    class Foo { 
        public:
            /*!
             * This is a QT Style comment, documenting bar. Note the '!'
             */
            int bar();
    
            /// 3 forward slashes can also be used to comment a function
            int baz(); 
    
            //! This will also work. 
            int foobar();
    
            /// To document enum members after their declaration,
            /// use a '<' to 'point backwards' to the declaration
            /// your are documenting
            enum Barnyard { 
                /** A feathered animal */
                Chicken, 
                Cow, ///< A large mammal. 
            };            
    
    };

##Adding Pages to Doxygen

To add prose documentation about the code you can use the page functionality. You can use one of two methods: 

###Documentation Files 
 
If you'd like, you can add prose in Markdown format into the <tt>doc/compiler</tt> directory. So long as this directory is scanned by Doxygen, it will be included as a related page. 

You can give this page a name to be referenced by adding a name tag:

    Header to my page {#MyPageName}
    ================= 

###Comment Prose

Any comment in the source code can become a page using the `\\page <label> <Title of Page>` command:

    /**
     * \\page MyPageName My Page 
     *
     * The Contents of my page
     *
     */

##Adding Links in Markdown

You can use Doxygen's link format inside of markdown code: 

    My other page {#OtherPage}
    =============
    
    The manual is divided into a number of subsections.
    
    - A link to my other page \\subpage MyPageName

[dox]: http://www.stack.nl/~dimitri/doxygen/
