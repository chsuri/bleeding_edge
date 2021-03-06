/*
 * Copyright 2012, the Dart project authors.
 * 
 * Licensed under the Eclipse Public License v1.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */
package com.google.dart.tools.core.utilities.net;

import com.google.dart.tools.core.test.util.TestUtilities;

import junit.framework.TestCase;

import java.net.URI;

public class URIUtilitiesTest extends TestCase {

  public void test_relativize_general() {
    URI url1 = URI.create("file://foo/bar");
    URI url2 = URI.create("file://foo/bar/baz");
    assertEquals(URI.create("baz"), URIUtilities.relativize(url1, url2));
  }

  public void test_relativize_withSpace() {
    URI url1 = URI.create("file://foo/bar");
    URI url2 = URI.create("file://foo/bar/woot%20there");
    assertEquals(URI.create("woot%20there"), URIUtilities.relativize(url1, url2));
  }

  public void test_URIUtilities_makeAbsolute_absolute() throws Exception {
    URI uri = TestUtilities.getLogFile().getAbsoluteFile().toURI();
    assertEquals(uri, URIUtilities.makeAbsolute(uri));
  }

  public void test_URIUtilities_makeAbsolute_null() throws Exception {
    assertNull(URIUtilities.makeAbsolute(null));
  }

  public void test_URIUtilities_makeAbsolute_relative_fileScheme() throws Exception {
    URI uri = new URI("file", "foo/bar", null);
    URI result = URIUtilities.makeAbsolute(uri);
    assertNotNull(result);
    assertTrue(result.toString(), result.toString().endsWith("/foo/bar"));
  }

  public void test_URIUtilities_makeAbsolute_relative_fileWithSpace() throws Exception {
    URI uri = new URI("file", "fo o/bar", null);
    URI result = URIUtilities.makeAbsolute(uri);
    assertNotNull(result);
    assertTrue(result.toString(), result.toString().endsWith("/fo%20o/bar"));
  }

  public void test_URIUtilities_makeAbsolute_relative_nonFileScheme() throws Exception {
    URI uri = new URI("http", "foo/bar", null);
    assertEquals(uri, URIUtilities.makeAbsolute(uri));
  }

  public void test_URIUtilities_makeAbsolute_relative_noScheme() throws Exception {
    URI uri = new URI(null, "foo/bar", null);
    URI result = URIUtilities.makeAbsolute(uri);
    assertNotNull(result);
    assertTrue(result.toString(), result.toString().endsWith("/foo/bar"));
  }

  public void test_URIUtilities_uriEncode_encodeSpace() throws Exception {
    String result = URIUtilities.uriEncode("foo bar");
    assertNotNull(result);
    assertEquals("foo%20bar", result);
  }

  public void test_URIUtilities_uriEncode_noEncode() throws Exception {
    String result = URIUtilities.uriEncode("fooBar");
    assertNotNull(result);
    assertEquals("fooBar", result);
  }

}
